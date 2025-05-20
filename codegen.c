/*
 * codegen.c - Assembly code generation module 
 * 
 * This module is responsible for translating the intermediate representation
 * (quadruples) into assembly code for the target architecture.
 */

#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global configuration
static CodeGenConfig config;

// Target architecture specific code generation functions
static void (*generatePrologue)(FILE* file);
static void (*generateEpilogue)(FILE* file);
static void (*generateQuad)(FILE* file, Quadruple* quad, SymbolTable* symtab);

// Helper data structures
typedef struct {
    char* name;
    int offset;
    int size;
    int isTemp;
} VariableInfo;

typedef struct {
    VariableInfo* vars;
    int count;
    int capacity;
} VarTable;

static VarTable varTable;
static int dataSize = 0;
static int labelCount = 0;
static int stringLiteralCount = 0;

// Forward declarations of architecture-specific functions
static void generatePrologueX86(FILE* file);
static void generateEpilogueX86(FILE* file);
static void generateQuadX86(FILE* file, Quadruple* quad, SymbolTable* symtab);

// Helper function to determine if a string is a valid identifier
static int isIdentifier(const char* str) {
    if (!str || !*str) return 0;
    
    // First character must be letter or underscore
    if (!isalpha(str[0]) && str[0] != '_') return 0;
    
    // Rest can be letters, digits, or underscore
    for (int i = 1; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0;
    }
    
    return 1;
}

// Helper function to determine if a string is a numeric constant
static int isNumericConstant(const char* str) {
    if (!str || !*str) return 0;
    
    // Skip leading negative sign if present
    int start = (str[0] == '-') ? 1 : 0;
    
    // Check for all digits (allowing one decimal point for floats)
    int hasDecimal = 0;
    for (int i = start; str[i]; i++) {
        if (str[i] == '.' && !hasDecimal) {
            hasDecimal = 1;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    
    return 1;
}

// Helper function to determine if a string is a temporary variable
static int isTempVar(const char* str) {
    return str && str[0] == 't' && isdigit(str[1]);
}

// Helper function to allocate a new string literal
static char* allocateStringLiteral(const char* str) {
    char* label = malloc(32);
    sprintf(label, "STR_%d", stringLiteralCount++);
    return label;
}

// Helper function to add a variable to the variable table
static void addVariable(const char* name, int size, int isTemp) {
    // Check if the variable already exists
    for (int i = 0; i < varTable.count; i++) {
        if (strcmp(varTable.vars[i].name, name) == 0) {
            return;  // Already exists
        }
    }
    
    // Expand the table if needed
    if (varTable.count >= varTable.capacity) {
        varTable.capacity *= 2;
        varTable.vars = realloc(varTable.vars, varTable.capacity * sizeof(VariableInfo));
        if (!varTable.vars) {
            fprintf(stderr, "Memory allocation failed in code generator\n");
            exit(1);
        }
    }
    
    // Add the new variable
    VariableInfo* var = &varTable.vars[varTable.count++];
    var->name = strdup(name);
    var->size = size;
    var->offset = dataSize;
    var->isTemp = isTemp;
    
    dataSize += size;
}

// Helper function to find a variable in the variable table
static VariableInfo* findVariable(const char* name) {
    for (int i = 0; i < varTable.count; i++) {
        if (strcmp(varTable.vars[i].name, name) == 0) {
            return &varTable.vars[i];
        }
    }
    return NULL;
}

// Helper function to get variable size based on type
static int getTypeSize(SymbolType type) {
    switch (type) {
        case TYPE_INTEGER:
            return 4;
        case TYPE_FLOAT:
            return 4;
        case TYPE_CHAR:
            return 1;
        case TYPE_STRING:
            return 4;  // Pointer to string
        case TYPE_VECTOR:
            return 4;  // Pointer to vector (array)
        default:
            return 4;  // Default to 4 bytes
    }
}

// Initialize the code generator
void initCodeGen(CodeGenConfig* cfg) {
    if (cfg) {
        memcpy(&config, cfg, sizeof(CodeGenConfig));
    } else {
        // Default configuration
        config.arch = ARCH_X86_64;
        config.format = FORMAT_GAS;
        config.outputFilename = "output.s";
        config.optimizationLevel = 1;
        config.debugInfo = 0;
    }
    
    // Initialize variable table
    varTable.capacity = 100;
    varTable.count = 0;
    varTable.vars = malloc(varTable.capacity * sizeof(VariableInfo));
    if (!varTable.vars) {
        fprintf(stderr, "Memory allocation failed in code generator\n");
        exit(1);
    }
    
    // Set architecture-specific functions
    switch (config.arch) {
        case ARCH_X86:
        case ARCH_X86_64:
            generatePrologue = generatePrologueX86;
            generateEpilogue = generateEpilogueX86;
            generateQuad = generateQuadX86;
            break;
        case ARCH_ARM:
            // To be implemented
            fprintf(stderr, "ARM code generation not yet implemented\n");
            exit(1);
            break;
    }
}

// First scan of quadruples to collect variables
static void scanQuadruples(QuadList* quadList, SymbolTable* symtab) {
    Quadruple* quad = quadList->head;
    
    while (quad) {
        // Check result
        if (quad->result && isIdentifier(quad->result) && !isNumericConstant(quad->result)) {
            SymbolEntry* entry = lookupSymbol(symtab, quad->result);
            if (entry) {
                addVariable(quad->result, getTypeSize(entry->type), 0);
            } else if (isTempVar(quad->result)) {
                addVariable(quad->result, 4, 1);  // Assume temp vars are 4 bytes
            }
        }
        
        // Check arg1
        if (quad->arg1 && isIdentifier(quad->arg1) && !isNumericConstant(quad->arg1)) {
            SymbolEntry* entry = lookupSymbol(symtab, quad->arg1);
            if (entry) {
                addVariable(quad->arg1, getTypeSize(entry->type), 0);
            } else if (isTempVar(quad->arg1)) {
                addVariable(quad->arg1, 4, 1);  // Assume temp vars are 4 bytes
            }
        }
        
        // Check arg2
        if (quad->arg2 && isIdentifier(quad->arg2) && !isNumericConstant(quad->arg2)) {
            SymbolEntry* entry = lookupSymbol(symtab, quad->arg2);
            if (entry) {
                addVariable(quad->arg2, getTypeSize(entry->type), 0);
            } else if (isTempVar(quad->arg2)) {
                addVariable(quad->arg2, 4, 1);  // Assume temp vars are 4 bytes
            }
        }
        
        quad = quad->next;
    }
}

// Generate assembly code from quadruples
void generateAssemblyCode(QuadList* quadList, SymbolTable* symtab) {
    FILE* file = fopen(config.outputFilename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open output file %s\n", config.outputFilename);
        return;
    }
    
    // Scan quadruples to collect variables
    scanQuadruples(quadList, symtab);
    
    // Generate prologue
    generatePrologue(file);
    
    // Generate code for each quadruple
    Quadruple* quad = quadList->head;
    while (quad) {
        generateQuad(file, quad, symtab);
        quad = quad->next;
    }
    
    // Generate epilogue
    generateEpilogue(file);
    
    fclose(file);
    printf("Generated assembly code in %s\n", config.outputFilename);
}

// Generate assembly code and write to a file
int generateAssemblyFile(QuadList* quadList, SymbolTable* symtab, const char* filename) {
    if (filename) {
        config.outputFilename = filename;
    }
    
    generateAssemblyCode(quadList, symtab);
    return 0;
}

// Free resources used by the code generator
void freeCodeGen() {
    // Free variable table
    for (int i = 0; i < varTable.count; i++) {
        free(varTable.vars[i].name);
    }
    free(varTable.vars);
    varTable.vars = NULL;
    varTable.count = 0;
    varTable.capacity = 0;
}

/******** Architecture-specific implementations ********/

/* X86 code generation */

// Generate x86 assembly prologue
static void generatePrologueX86(FILE* file) {
    // Data section
    fprintf(file, "# Assembly generated by compiler\n");
    fprintf(file, ".data\n");
    
    // Declare variables
    for (int i = 0; i < varTable.count; i++) {
        VariableInfo* var = &varTable.vars[i];
        fprintf(file, "%s: .space %d\n", var->name, var->size);
    }
    
    // Text section
    fprintf(file, "\n.text\n");
    fprintf(file, ".globl main\n");
    fprintf(file, "main:\n");
    fprintf(file, "    # Function prologue\n");
    fprintf(file, "    push %%ebp\n");
    fprintf(file, "    mov %%esp, %%ebp\n");
    
    // Reserve space for local variables
    if (dataSize > 0) {
        fprintf(file, "    sub $%d, %%esp\n", dataSize);
    }
}

// Generate x86 assembly epilogue
static void generateEpilogueX86(FILE* file) {
    fprintf(file, "    # Function epilogue\n");
    fprintf(file, "    mov $0, %%eax\n");  // Return value
    fprintf(file, "    mov %%ebp, %%esp\n");
    fprintf(file, "    pop %%ebp\n");
    fprintf(file, "    ret\n");
}

// Generate x86 assembly for a quadruple
static void generateQuadX86(FILE* file, Quadruple* quad, SymbolTable* symtab) {
    // Skip NOPs (marked by optimizer)
    if (quad->op == QUAD_LABEL && 
        quad->arg1 && strcmp(quad->arg1, "NOP") == 0 &&
        quad->result && strcmp(quad->result, "NOP") == 0) {
        return;
    }
    
    // Generate a comment for the quadruple
    fprintf(file, "    # ");
    if (quad->result) fprintf(file, "%s = ", quad->result);
    
    switch (quad->op) {
        case QUAD_ASSIGN:
            fprintf(file, "%s", quad->arg1);
            break;
        case QUAD_ADD:
            fprintf(file, "%s + %s", quad->arg1, quad->arg2);
            break;
        case QUAD_SUB:
            fprintf(file, "%s - %s", quad->arg1, quad->arg2);
            break;
        case QUAD_MULT:
            fprintf(file, "%s * %s", quad->arg1, quad->arg2);
            break;
        case QUAD_DIV:
            fprintf(file, "%s / %s", quad->arg1, quad->arg2);
            break;
        case QUAD_IF_GT:
            fprintf(file, "if %s > %s goto %s", quad->arg1, quad->arg2, quad->result);
            break;
        case QUAD_IF_LT:
            fprintf(file, "if %s < %s goto %s", quad->arg1, quad->arg2, quad->result);
            break;
        case QUAD_IF_GE:
            fprintf(file, "if %s >= %s goto %s", quad->arg1, quad->arg2, quad->result);
            break;
        case QUAD_IF_LE:
            fprintf(file, "if %s <= %s goto %s", quad->arg1, quad->arg2, quad->result);
            break;
        case QUAD_IF_EQ:
            fprintf(file, "if %s == %s goto %s", quad->arg1, quad->arg2, quad->result);
            break;
        case QUAD_IF_NE:
            fprintf(file, "if %s != %s goto %s", quad->arg1, quad->arg2, quad->result);
            break;
        case QUAD_IF_FALSE:
            fprintf(file, "if !%s goto %s", quad->arg1, quad->result ? quad->result : "next_label");
            break;
        case QUAD_GOTO:
            fprintf(file, "goto %s", quad->result);
            break;
        case QUAD_LABEL:
            fprintf(file, "LABEL %s", quad->result);
            break;
        case QUAD_READ:
            fprintf(file, "READ %s into %s", quad->arg1, quad->result);
            break;
        case QUAD_WRITE:
            fprintf(file, "WRITE %s with format %s", quad->arg2, quad->arg1);
            break;
        default:
            fprintf(file, "UNKNOWN");
            break;
    }
    fprintf(file, "\n");
    
    // Generate actual x86 assembly based on the quadruple type
    switch (quad->op) {
        case QUAD_ASSIGN:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %s\n", quad->arg1, quad->result);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
                fprintf(file, "    mov %%eax, %s\n", quad->result);
            }
            break;
            
        case QUAD_ADD:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    add $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    add %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    mov %%eax, %s\n", quad->result);
            break;
            
        case QUAD_SUB:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    sub $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    sub %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    mov %%eax, %s\n", quad->result);
            break;
            
        case QUAD_MULT:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    mov $%s, %%ebx\n", quad->arg2);
                fprintf(file, "    imul %%ebx, %%eax\n");
            } else {
                fprintf(file, "    imul %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    mov %%eax, %s\n", quad->result);
            break;
            
        case QUAD_DIV:
            fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            fprintf(file, "    cdq\n");  // Sign-extend eax into edx:eax
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    mov $%s, %%ecx\n", quad->arg2);
                fprintf(file, "    idiv %%ecx\n");
            } else {
                fprintf(file, "    idiv %s\n", quad->arg2);
            }
            
            fprintf(file, "    mov %%eax, %s\n", quad->result);
            break;
            
        case QUAD_LABEL:
            fprintf(file, "%s:\n", quad->result);
            break;
            
        case QUAD_GOTO:
            fprintf(file, "    jmp %s\n", quad->result);
            break;
            
        case QUAD_IF_EQ:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    cmp $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    cmp %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    je %s\n", quad->result);
            break;
            
        case QUAD_IF_NE:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    cmp $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    cmp %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    jne %s\n", quad->result);
            break;
            
        case QUAD_IF_LT:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    cmp $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    cmp %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    jl %s\n", quad->result);
            break;
            
        case QUAD_IF_LE:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    cmp $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    cmp %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    jle %s\n", quad->result);
            break;
            
        case QUAD_IF_GT:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    cmp $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    cmp %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    jg %s\n", quad->result);
            break;
            
        case QUAD_IF_GE:
            if (isNumericConstant(quad->arg1)) {
                fprintf(file, "    mov $%s, %%eax\n", quad->arg1);
            } else {
                fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            }
            
            if (isNumericConstant(quad->arg2)) {
                fprintf(file, "    cmp $%s, %%eax\n", quad->arg2);
            } else {
                fprintf(file, "    cmp %s, %%eax\n", quad->arg2);
            }
            
            fprintf(file, "    jge %s\n", quad->result);
            break;
            
        case QUAD_IF_FALSE:
            fprintf(file, "    mov %s, %%eax\n", quad->arg1);
            fprintf(file, "    test %%eax, %%eax\n");
            // Handle the case when result is empty or NULL
            if (quad->result && strlen(quad->result) > 0) {
                fprintf(file, "    je %s\n", quad->result);
            } else {
                // Generate a new label for the next instruction
                char nextLabel[20];
                sprintf(nextLabel, "L_next_%d", labelCount++);
                fprintf(file, "    je %s\n", nextLabel);
                fprintf(file, "%s:\n", nextLabel);
            }
            break;
            
        case QUAD_READ:
            // Basic implementation - will need to be extended for complex reads
            fprintf(file, "    # Input not implemented in basic x86 assembly\n");
            fprintf(file, "    # Placeholder: Read into %s\n", quad->result);
            break;
            
        case QUAD_WRITE:
            // Basic implementation - will need to be extended for complex output
            fprintf(file, "    # Output not implemented in basic x86 assembly\n");
            fprintf(file, "    # Placeholder: Print %s\n", quad->arg2);
            break;
            
        default:
            fprintf(file, "    # Unsupported operation\n");
            break;
    }
} 