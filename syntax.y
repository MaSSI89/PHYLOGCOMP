%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "quad.h"

int yylex();
int line = 1;
int column = 1;
void yyerror(const char *s);

// Global symbol table
SymbolTable* symtab;
// Global quadruple list
QuadList* quad_list;
// Current type being processed in declarations
SymbolType currentType;

// External declarations
extern char* yytext;
extern int tempCount;  // From quad.c
%}

%start program

%token mc_ident mc_data mc_endp mc_code mc_comment mc_const mc_vector mc_integ mc_float mc_char mc_string
%token mc_deuxp mc_pvg mc_affectation mc_crocheto mc_crochetf mc_vg mc_separateur mc_paro mc_parf 
%token mc_plus mc_moins mc_mult mc_div
%token mc_read mc_display mc_if mc_else mc_for mc_sup mc_inf mc_supeg mc_infeg mc_egal mc_diff mc_adresse
%token INT_CONST mc_charactere mc_chaine mc_cst mc_or mc_and mc_start

%union {
    int int_val;
    char char_val;
    char* str_val;
    struct ExprValue expr_val;
}

%type <str_val> mc_ident
%type <int_val> INT_CONST mc_cst
%type <char_val> mc_charactere
%type <str_val> mc_chaine
%type <type_val> VAR_TYPE
%type <expr_val> EXPRESSION CONDITION VALEURS

%left mc_or mc_and
%left mc_egal mc_diff
%left mc_sup mc_inf mc_supeg mc_infeg
%left mc_plus mc_moins
%left mc_mult mc_div
%right mc_affectation

%%

program: 
    comment_list mc_ident DATA_SECTION CODE_SECTION mc_endp {
        printf("Compilation successful!\n");
        printSymbolTable(symtab);
        printQuadList(quad_list);
        YYACCEPT;
    }
;

comment_list:
    /* empty */
    | comment_list mc_comment
;

DATA_SECTION:
    comment_list mc_data DECLARATIONS comment_list mc_endp
;

DECLARATIONS:
    /* empty */
    | DECLARATIONS DEC
;

DEC:
    comment_list VAR_TYPE mc_deuxp LISTE_VAR mc_pvg
    | comment_list DEC_CONST
    | comment_list DEC_VECTOR
;

VAR_TYPE:
    mc_integ { currentType = TYPE_INTEGER; }
    | mc_float { currentType = TYPE_FLOAT; }
    | mc_char { currentType = TYPE_CHAR; }
    | mc_string { currentType = TYPE_STRING; }
;

LISTE_VAR:
    mc_ident {
        if (!insertSymbol(symtab, $1, currentType, line, column)) {
            // Error already reported by insertSymbol
        }
    }
    | mc_ident mc_separateur LISTE_VAR {
        if (!insertSymbol(symtab, $1, currentType, line, column)) {
            // Error already reported by insertSymbol
        }
    }
;

DEC_CONST:
    mc_const mc_deuxp mc_ident mc_affectation VALEURS mc_pvg {
        if (!insertSymbol(symtab, $3, $5.type, line, column)) {
            // Error already reported by insertSymbol
        } else {
            // Mark as constant and initialized
            SymbolEntry* entry = lookupSymbol(symtab, $3);
            if (entry) {
                entry->isConstant = 1;
                entry->isInitialized = 1;
                
                // Set value based on type
                switch (entry->type) {
                    case TYPE_INTEGER:
                        entry->value.intValue = $5.value.intValue;
                        break;
                    case TYPE_FLOAT:
                        entry->value.floatValue = $5.value.floatValue;
                        break;
                    case TYPE_CHAR:
                        entry->value.charValue = $5.value.charValue;
                        break;
                    case TYPE_STRING:
                        entry->value.stringValue = strdup($5.value.stringValue);
                        break;
                    default:
                        break;
                }
            }
        }
    }
;

DEC_VECTOR:
    mc_vector mc_deuxp mc_ident mc_crocheto INT_CONST mc_vg INT_CONST mc_deuxp VAR_TYPE mc_crochetf mc_pvg {
        if (!insertSymbol(symtab, $3, TYPE_VECTOR, line, column)) {
            // Error already reported by insertSymbol
        } else {
            // Set vector dimensions and element type
            SymbolEntry* entry = lookupSymbol(symtab, $3);
            if (entry) {
                entry->value.vectorInfo.rows = $5;
                entry->value.vectorInfo.cols = $7;
                entry->value.vectorInfo.elementType = currentType;
            }
        }
    }
;

VALEURS:
    mc_charactere {
        $$.type = TYPE_CHAR;
        $$.value.charValue = $1;
        sprintf($$.valueStr, "'%c'", $1);
    }
    | mc_chaine {
        $$.type = TYPE_STRING;
        $$.value.stringValue = $1;
        sprintf($$.valueStr, "%s", $1);
    }
    | INT_CONST {
        $$.type = TYPE_INTEGER;
        $$.value.intValue = $1;
        sprintf($$.valueStr, "%d", $1);
    }
    | mc_cst {
        $$.type = TYPE_INTEGER;
        $$.value.intValue = $1;
        sprintf($$.valueStr, "%d", $1);
    }
;

CODE_SECTION:
    comment_list mc_code INSTRUCTIONS comment_list mc_endp
;

INSTRUCTIONS:
    /* empty */
    | INSTRUCTIONS INSTRUCTION
;

INSTRUCTION:
    comment_list INSTRUCTION_AFFECTATION
    | comment_list INSTRUCTION_LIRE
    | comment_list INSTRUCTION_ECRIRE
    | comment_list INSTRUCTION_CONDITION
    | comment_list INSTRUCTION_BOUCLE
    | comment_list mc_pvg  /* Handle empty instructions with semicolons */
;

INSTRUCTION_AFFECTATION:
    mc_ident mc_affectation EXPRESSION mc_pvg {
        SymbolEntry* entry = lookupSymbol(symtab, $1);
        if (!entry) {
            fprintf(stderr, " ------------SEMANTIC Error: Undefined variable '%s' at line %d ------------\n", $1, line);
        } else {
            entry->isInitialized = 1;
            
            // Check type compatibility
            if (!areTypesCompatible(entry->type, $3.type, '=')) {
                fprintf(stderr, "-------------SEMANTIC Error: Type mismatch in assignment to '%s' at line %d -------------\n", $1, line);
            } else {
                // Set the value based on types
                switch (entry->type) {
                    case TYPE_INTEGER:
                        if ($3.type == TYPE_FLOAT) {
                            entry->value.intValue = (int)$3.value.floatValue;
                        } else if ($3.type == TYPE_CHAR) {
                            entry->value.intValue = (int)$3.value.charValue;
                        } else {
                            entry->value.intValue = $3.value.intValue;
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, $3.valueStr, NULL, $1);
                        break;
                    case TYPE_FLOAT:
                        if ($3.type == TYPE_INTEGER) {
                            entry->value.floatValue = (float)$3.value.intValue;
                        } else if ($3.type == TYPE_CHAR) {
                            entry->value.floatValue = (float)$3.value.charValue;
                        } else {
                            entry->value.floatValue = $3.value.floatValue;
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, $3.valueStr, NULL, $1);
                        break;
                    case TYPE_CHAR:
                        if ($3.type == TYPE_INTEGER) {
                            entry->value.charValue = (char)$3.value.intValue;
                        } else {
                            entry->value.charValue = $3.value.charValue;
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, $3.valueStr, NULL, $1);
                        break;
                    case TYPE_STRING:
                        if ($3.type == TYPE_STRING) {
                            if (entry->value.stringValue) {
                                free(entry->value.stringValue);
                            }
                            entry->value.stringValue = strdup($3.value.stringValue);
                        } else {
                            fprintf(stderr, "------------SEMANTIC Error: Cannot assign non-string to string at line %d ------------\n", line);
                        }
                        // Generate quadruple for assignment
                        addQuad(quad_list, QUAD_ASSIGN, $3.valueStr, NULL, $1);
                        break;
                    default:
                        break;
                }
            }
        }
    }
;

EXPRESSION:
    EXPRESSION mc_plus EXPRESSION {
        if ($1.type == TYPE_STRING || $3.type == TYPE_STRING) {
            // String concatenation
            $$.type = TYPE_STRING;
            int len = strlen($1.value.stringValue) + strlen($3.value.stringValue) + 1;
            $$.value.stringValue = malloc(len);
            strcpy($$.value.stringValue, $1.value.stringValue);
            strcat($$.value.stringValue, $3.value.stringValue);
            sprintf($$.valueStr, "%s", $$.value.stringValue);
            
            // Generate quadruple
            char* temp = genBinaryOp(quad_list, QUAD_ADD, $1.valueStr, $3.valueStr);
            strcpy($$.valueStr, temp);
            free(temp);
        } else if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) {
            // Float addition
            $$.type = TYPE_FLOAT;
            $$.value.floatValue = convertToFloat($1) + convertToFloat($3);
            sprintf($$.valueStr, "%f", $$.value.floatValue);
            
            // Generate quadruple
            char* temp = genBinaryOp(quad_list, QUAD_ADD, $1.valueStr, $3.valueStr);
            strcpy($$.valueStr, temp);
            free(temp);
        } else {
            // Integer addition
            $$.type = TYPE_INTEGER;
            $$.value.intValue = convertToInt($1) + convertToInt($3);
            sprintf($$.valueStr, "%d", $$.value.intValue);
            
            // Generate quadruple
            char* temp = genBinaryOp(quad_list, QUAD_ADD, $1.valueStr, $3.valueStr);
            strcpy($$.valueStr, temp);
            free(temp);
        }
    }
    | EXPRESSION mc_moins EXPRESSION {
        if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) {
            // Float subtraction
            $$.type = TYPE_FLOAT;
            $$.value.floatValue = convertToFloat($1) - convertToFloat($3);
            sprintf($$.valueStr, "%f", $$.value.floatValue);
        } else {
            // Integer subtraction
            $$.type = TYPE_INTEGER;
            $$.value.intValue = convertToInt($1) - convertToInt($3);
            sprintf($$.valueStr, "%d", $$.value.intValue);
        }
        
        // Generate quadruple
        char* temp = genBinaryOp(quad_list, QUAD_SUB, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_mult EXPRESSION {
        if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) {
            // Float multiplication
            $$.type = TYPE_FLOAT;
            $$.value.floatValue = convertToFloat($1) * convertToFloat($3);
            sprintf($$.valueStr, "%f", $$.value.floatValue);
        } else {
            // Integer multiplication
            $$.type = TYPE_INTEGER;
            $$.value.intValue = convertToInt($1) * convertToInt($3);
            sprintf($$.valueStr, "%d", $$.value.intValue);
        }
        
        // Generate quadruple
        char* temp = genBinaryOp(quad_list, QUAD_MULT, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_div EXPRESSION {
        // Check for division by zero
        if (($3.type == TYPE_INTEGER && $3.value.intValue == 0) ||
            ($3.type == TYPE_FLOAT && $3.value.floatValue == 0.0)) {
            fprintf(stderr, "------------ SEMANTIC Error: Division by zero at line %d ------------\n", line);
            $$.type = TYPE_INTEGER;
            $$.value.intValue = 0;
            sprintf($$.valueStr, "0");
        } else if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) {
            // Float division
            $$.type = TYPE_FLOAT;
            $$.value.floatValue = convertToFloat($1) / convertToFloat($3);
            sprintf($$.valueStr, "%f", $$.value.floatValue);
        } else {
            // Integer division
            $$.type = TYPE_INTEGER;
            $$.value.intValue = convertToInt($1) / convertToInt($3);
            sprintf($$.valueStr, "%d", $$.value.intValue);
        }
        
        // Generate quadruple
        char* temp = genBinaryOp(quad_list, QUAD_DIV, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_sup EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        $$.value.intValue = (convertToFloat($1) > convertToFloat($3)) ? 1 : 0;
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_GT, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_inf EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        $$.value.intValue = (convertToFloat($1) < convertToFloat($3)) ? 1 : 0;
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_LT, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_supeg EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        $$.value.intValue = (convertToFloat($1) >= convertToFloat($3)) ? 1 : 0;
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_GE, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_infeg EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        $$.value.intValue = (convertToFloat($1) <= convertToFloat($3)) ? 1 : 0;
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_LE, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_egal EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        
        if ($1.type == TYPE_STRING && $3.type == TYPE_STRING) {
            $$.value.intValue = (strcmp($1.value.stringValue, $3.value.stringValue) == 0) ? 1 : 0;
        } else {
            $$.value.intValue = (convertToFloat($1) == convertToFloat($3)) ? 1 : 0;
        }
        
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_EQ, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_diff EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        
        if ($1.type == TYPE_STRING && $3.type == TYPE_STRING) {
            $$.value.intValue = (strcmp($1.value.stringValue, $3.value.stringValue) != 0) ? 1 : 0;
        } else {
            $$.value.intValue = (convertToFloat($1) != convertToFloat($3)) ? 1 : 0;
        }
        
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate comparison quadruple
        char* temp = genBinaryOp(quad_list, QUAD_IF_NE, $1.valueStr, $3.valueStr);
        strcpy($$.valueStr, temp);
        free(temp);
    }
    | EXPRESSION mc_and EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        $$.value.intValue = (convertToInt($1) && convertToInt($3)) ? 1 : 0;
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate logical AND quadruple (using IF_NE with 0)
        char* temp1 = generateTemp();
        sprintf(temp1, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, $1.valueStr, NULL, temp1);
        
        char* temp2 = generateTemp();
        sprintf(temp2, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, $3.valueStr, NULL, temp2);
        
        char* resultTemp = generateTemp();
        sprintf(resultTemp, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, "0", NULL, resultTemp);
        
        int labelTrue = genLabel(quad_list);
        addQuad(quad_list, QUAD_IF_EQ, temp1, "0", resultTemp);
        addQuad(quad_list, QUAD_IF_EQ, temp2, "0", resultTemp);
        addQuad(quad_list, QUAD_ASSIGN, "1", NULL, resultTemp);
        
        strcpy($$.valueStr, resultTemp);
        free(temp1);
        free(temp2);
        free(resultTemp);
    }
    | EXPRESSION mc_or EXPRESSION {
        $$.type = TYPE_INTEGER;  // Boolean result (0 or 1)
        $$.value.intValue = (convertToInt($1) || convertToInt($3)) ? 1 : 0;
        sprintf($$.valueStr, "%d", $$.value.intValue);
        
        // Generate logical OR quadruple (using IF_NE with 0)
        char* temp1 = generateTemp();
        sprintf(temp1, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, $1.valueStr, NULL, temp1);
        
        char* temp2 = generateTemp();
        sprintf(temp2, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, $3.valueStr, NULL, temp2);
        
        char* resultTemp = generateTemp();
        sprintf(resultTemp, "t%d", tempCount++);
        addQuad(quad_list, QUAD_ASSIGN, "1", NULL, resultTemp);
        
        int labelFalse = genLabel(quad_list);
        addQuad(quad_list, QUAD_IF_NE, temp1, "0", resultTemp);
        addQuad(quad_list, QUAD_IF_NE, temp2, "0", resultTemp);
        addQuad(quad_list, QUAD_ASSIGN, "0", NULL, resultTemp);
        
        strcpy($$.valueStr, resultTemp);
        free(temp1);
        free(temp2);
        free(resultTemp);
    }
    | mc_paro EXPRESSION mc_parf {
        $$ = $2;  // Just pass the expression value up
    }
    | mc_ident {
        SymbolEntry* entry = lookupSymbol(symtab, $1);
        if (!entry) {
            fprintf(stderr, "-------------SEMANTIC Error: Undefined variable '%s' at line %d -------------\n", $1, line);
            $$.type = TYPE_INTEGER;
            $$.value.intValue = 0;
            sprintf($$.valueStr, "0");
        } else {
            entry->isUsed = 1;
            
            if (!entry->isInitialized) {
                fprintf(stderr, "Warning: Uninitialized variable '%s' used at line %d\n", $1, line);
            }
            
            // Set the expression value based on variable type
            $$.type = entry->type;
            switch (entry->type) {
                case TYPE_INTEGER:
                    $$.value.intValue = entry->value.intValue;
                    sprintf($$.valueStr, "%d", $$.value.intValue);
                    break;
                case TYPE_FLOAT:
                    $$.value.floatValue = entry->value.floatValue;
                    sprintf($$.valueStr, "%f", $$.value.floatValue);
                    break;
                case TYPE_CHAR:
                    $$.value.charValue = entry->value.charValue;
                    sprintf($$.valueStr, "'%c'", $$.value.charValue);
                    break;
                case TYPE_STRING:
                    if (entry->value.stringValue) {
                        $$.value.stringValue = strdup(entry->value.stringValue);
                        sprintf($$.valueStr, "%s", $$.value.stringValue);
                    } else {
                        $$.value.stringValue = strdup("");
                        sprintf($$.valueStr, "\"\"");
                    }
                    break;
                default:
                    $$.type = TYPE_INTEGER;
                    $$.value.intValue = 0;
                    sprintf($$.valueStr, "0");
                    break;
            }
            
            // For symbol reference, just use the name
            strcpy($$.valueStr, $1);
        }
    }
    | INT_CONST {
        $$.type = TYPE_INTEGER;
        $$.value.intValue = $1;
        sprintf($$.valueStr, "%d", $1);
    }
    | mc_charactere {
        $$.type = TYPE_CHAR;
        $$.value.charValue = $1;
        sprintf($$.valueStr, "'%c'", $1);
    }
    | mc_chaine {
        $$.type = TYPE_STRING;
        $$.value.stringValue = $1;
        sprintf($$.valueStr, "%s", $1);
    }
    | mc_cst {
        $$.type = TYPE_INTEGER;
        $$.value.intValue = $1;
        sprintf($$.valueStr, "%d", $1);
    }
;

INSTRUCTION_LIRE:
    mc_read mc_paro mc_chaine mc_deuxp mc_adresse mc_ident mc_parf mc_pvg {
        SymbolEntry* entry = lookupSymbol(symtab, $6);
        if (!entry) {
            fprintf(stderr, "--------------SEMANTIC Error: Undefined variable '%s' at line %d--------------\n", $6, line);
        } else {
            entry->isInitialized = 1;
            
            // Generate quadruple for read operation
            genReadQuad(quad_list, $3, $6);
        }
    }
;

INSTRUCTION_ECRIRE:
    mc_display mc_paro mc_chaine mc_deuxp mc_ident mc_parf mc_pvg {
        SymbolEntry* entry = lookupSymbol(symtab, $5);
        if (!entry) {
            fprintf(stderr, "---------------SEMANTIC Error: Undefined variable '%s' at line %d T column %d---------------\n", $5, line, column);
        } else {
            entry->isUsed = 1;
            
            // Generate quadruple for write operation
            genWriteQuad(quad_list, $3, $5);
        }
    }
;

INSTRUCTION_CONDITION:
    STRUCTURE_CONDITIONELLE
;

STRUCTURE_CONDITIONELLE:
    mc_if mc_paro CONDITION mc_parf mc_deuxp INSTRUCTIONS mc_endp {
        // Single if statement (no else)
        int ifCondLabel = genLabel(quad_list);
        // Generate conditional jump
        if ($3.type == TYPE_INTEGER) {
            addQuad(quad_list, QUAD_IF_FALSE, $3.valueStr, NULL, "");
        }
    }
    | mc_if mc_paro CONDITION mc_parf mc_deuxp INSTRUCTIONS mc_else mc_deuxp INSTRUCTIONS mc_endp {
        // If-else statement
        int elseLabel = genLabel(quad_list);
        int endIfLabel = genLabel(quad_list);
        
        // Generate conditional jumps
        if ($3.type == TYPE_INTEGER) {
            // If condition is false, jump to else
            addQuad(quad_list, QUAD_IF_FALSE, $3.valueStr, NULL, "");
            // At end of if block, jump to end
            genGoto(quad_list, endIfLabel);
            // Place else label
            genLabel(quad_list);
            // Place end label after else block
            genLabel(quad_list);
        }
    }
;

CONDITION:
    EXPRESSION {
        $$ = $1;  // Just pass the expression value up
        
        // Ensure condition is treated as a boolean
        if ($$.type != TYPE_INTEGER) {
            $$.type = TYPE_INTEGER;
            $$.value.intValue = ($$.value.intValue != 0) ? 1 : 0;
            sprintf($$.valueStr, "%d", $$.value.intValue);
        }
    }
;

INSTRUCTION_BOUCLE:
    mc_for mc_paro mc_ident mc_deuxp INT_CONST mc_deuxp mc_ident mc_parf INSTRUCTION mc_endp {
        // For loop 
        // First identifier is the loop counter
        SymbolEntry* counterEntry = lookupSymbol(symtab, $3);
        if (!counterEntry) {
            fprintf(stderr, " ------------SEMANTIC Error: Undefined variable '%s' at line %d COLUMN %d ------------\n", $3, line, column);
        } else {
            counterEntry->isInitialized = 1;
            counterEntry->isUsed = 1;
            
            // Set counter to initial value
            addQuad(quad_list, QUAD_ASSIGN, "0", NULL, $3);
            
            // Second constant is the limit
            char limitStr[20];
            sprintf(limitStr, "%d", $5);
            
            // Third identifier is the condition variable
            SymbolEntry* condEntry = lookupSymbol(symtab, $7);
            if (!condEntry) {
                fprintf(stderr, " ------------SEMANTIC Error: Undefined variable '%s' at line %d COLUMN %d ------------\n", $7, line, column);
            } else {
                condEntry->isUsed = 1;
                
                // Start of loop label
                int startLabel = genLabel(quad_list);
                
                // Condition: counter < limit
                char* tempResult = generateTemp();
                addQuad(quad_list, QUAD_IF_LT, $3, limitStr, tempResult);
                
                // If condition is false, exit the loop
                int endLabel = genLabel(quad_list);
                addQuad(quad_list, QUAD_IF_FALSE, tempResult, NULL, "");
                
                // Body of the loop
                // (INSTRUCTION is already parsed)
                
                // Increment the counter
                char* tempInc = generateTemp();
                addQuad(quad_list, QUAD_ADD, $3, "1", tempInc);
                addQuad(quad_list, QUAD_ASSIGN, tempInc, NULL, $3);
                
                // Jump back to start
                genGoto(quad_list, startLabel);
                
                // End of loop label
                genLabel(quad_list);
                
                free(tempResult);
                free(tempInc);
            }
        }
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "SYNTAX Error: %s at line %d column %d\n", s, line, column);
}

int main() {
    printf("Starting compilation...\n");
    
    // Initialize symbol table
    symtab = createSymbolTable();
    
    // Initialize quadruple list
    quad_list = createQuadList();
    
    int result = yyparse();
    if (result == 0) {
        printf("Compilation successful!\n");
    }
    
    // Free memory
    freeSymbolTable(symtab);
    freeQuadList(quad_list);
    
    return result;
}

int yywrap() {
    return 1;
}