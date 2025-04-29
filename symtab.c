#include "symtab.h"
#include "expr_value.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Add this prototype at the top of the file
const char* getTypeName(SymbolType type);

// The global variable is defined in syntax.y, we just reference it here
extern SymbolType currentType;

// Create a new symbol table
SymbolTable* createSymbolTable() {
    SymbolTable* symTab = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (symTab == NULL) {
        fprintf(stderr, "Memory allocation failed for symbol table\n");
        exit(1);
    }
    
    // Initialize all hash table entries to NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        symTab->hashTable[i] = NULL;
    }
    symTab->size = 0;
    return symTab;
}

// Hash function - maps first letter to 0-25 (A-Z)
unsigned int hash(const char* name) {
    if (name == NULL || name[0] == '\0') {
        fprintf(stderr, "Warning: Empty name passed to hash function\n");
        return 0;
    }
    
    char firstChar = toupper(name[0]);
    
    if (firstChar >= 'A' && firstChar <= 'Z') {
        return firstChar - 'A';
    }
    
    fprintf(stderr, "Warning: Non-alphabetic first character: '%c'\n", name[0]);
    return 0;
}

// Create a new symbol entry
SymbolEntry* createEntry(const char* name, SymbolType type, int line, int column) {
    if (name == NULL) {
        fprintf(stderr, "Error: Attempt to create entry with NULL name\n");
        return NULL;
    }
    
    SymbolEntry* entry = (SymbolEntry*)malloc(sizeof(SymbolEntry));
    if (entry == NULL) {
        fprintf(stderr, "Memory allocation failed for symbol entry\n");
        exit(1);
    }
    
    entry->name = strdup(name);
    if (entry->name == NULL) {
        fprintf(stderr, "Memory allocation failed for symbol name\n");
        free(entry);
        return NULL;
    }
    
    entry->type = type;
    entry->line = line;
    entry->column = column;
    entry->isInitialized = 0;
    entry->isConstant = (type == TYPE_CONST);
    entry->isUsed = 0;
    entry->next = NULL;
    
    // Initialize value based on type
    switch (type) {
        case TYPE_INTEGER:
            entry->value.intValue = 0;
            break;
        case TYPE_FLOAT:
            entry->value.floatValue = 0.0;
            break;
        case TYPE_STRING:
            entry->value.stringValue = NULL;
            break;
        case TYPE_CHAR:
            entry->value.charValue = '\0';
            break;
        case TYPE_VECTOR:
            entry->value.vectorInfo.rows = 0;
            entry->value.vectorInfo.cols = 0;
            entry->value.vectorInfo.elementType = TYPE_INTEGER;
            break;
        default:
            break;
    }
    
    return entry;
}

// Insert a symbol into the table
int insertSymbol(SymbolTable* symTab, const char* name, SymbolType type, int line, int column) {
    // Check for NULL pointers
    if (symTab == NULL || name == NULL) {
        fprintf(stderr, " ------------SEMANTIC Error: NULL pointer passed to insertSymbol ------------\n");
        return 0;
    }
    
    // Check if symbol already exists
    if (lookupSymbol(symTab, name) != NULL) {
        fprintf(stderr, " ------------SEMANTIC Error: Symbol '%s' already defined at line %d, column %d ------------\n", name, line, column);
        return 0;
    }
    
    unsigned int index = hash(name);
    
    // Create a new entry
    SymbolEntry* entry = (SymbolEntry*)malloc(sizeof(SymbolEntry));
    if (entry == NULL) {
        fprintf(stderr, "Memory allocation failed for symbol entry\n");
        exit(1);
    }

    // Initialize entry
    entry->name = strdup(name);
    entry->type = type;
    entry->line = line;
    entry->column = column;
    entry->isInitialized = 0;
    
    // Set isConstant based on the parent rule that's calling this function
    // Constants are stored as their base type (INTEGER, FLOAT, etc) but with isConstant=1
    entry->isConstant = 0;  // Default to non-constant
    entry->isUsed = 0;
    
    // Initialize value based on type
    switch (type) {
        case TYPE_INTEGER:
            entry->value.intValue = 0;
            break;
        case TYPE_FLOAT:
            entry->value.floatValue = 0.0;
            break;
        case TYPE_CHAR:
            entry->value.charValue = '\0';
            break;
        case TYPE_STRING:
            entry->value.stringValue = NULL;
            break;
        case TYPE_VECTOR:
            entry->value.vectorInfo.rows = 0;
            entry->value.vectorInfo.cols = 0;
            entry->value.vectorInfo.elementType = TYPE_INTEGER;
            break;
        default:
            break;
    }
    
    // Insert at the beginning of the list
    entry->next = symTab->hashTable[index];
    symTab->hashTable[index] = entry;
    symTab->size++;
    
    return 1;  // Successfully inserted
}

// Look up a symbol in the table
SymbolEntry* lookupSymbol(SymbolTable* symTab, const char* name) {
    unsigned int index = hash(name);
    SymbolEntry* current = symTab->hashTable[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;  // Not found
}

// Print the contents of the symbol table
void printSymbolTable(SymbolTable* symTab) {
    printf("\n=== Symbol Table ===\n");
    printf("Hash  %-15s %-10s %-20s %-8s %-8s %-8s\n", 
           "Name", "Type", "Value", "Line", "Column", "Const");
    printf("-------------------------------------------------------------------\n");
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        SymbolEntry* current = symTab->hashTable[i];
        
        if (current != NULL) {
            printf("[%c]:\n", 'A' + i);
            
            while (current != NULL) {
                printf("     %-19s ", current->name);
                
                // Print type
                switch (current->type) {
                    case TYPE_INTEGER:
                        printf("%-10s ", "INTEGER");
                        printf("%-20d ", current->value.intValue);
                        break;
                    case TYPE_FLOAT:
                        printf("%-10s ", "FLOAT");
                        printf("%-20.2f ", current->value.floatValue);
                        break;
                    case TYPE_CHAR:
                        printf("%-10s ", "CHAR");
                        printf("%-20c ", current->value.charValue);
                        break;
                    case TYPE_STRING:
                        printf("%-10s ", "STRING");
                        if (current->value.stringValue) {
                            // Truncate string if too long for display
                            char displayStr[18] = {0};
                            strncpy(displayStr, current->value.stringValue, 17);
                            if (strlen(current->value.stringValue) > 17) {
                                strcat(displayStr, "...");
                            }
                            printf("%-20s ", displayStr);
                        } else {
                            printf("%-20s ", "NULL");
                        }
                        break;
                    case TYPE_BOOLEAN:
                        printf("%-10s ", "BOOLEAN");
                        printf("%-20s ", "");
                        break;
                    case TYPE_VECTOR:
                        printf("%-10s ", "VECTOR");
                        printf("[%d,%d:%-10s] ", 
                               current->value.vectorInfo.rows,
                               current->value.vectorInfo.cols,
                               getTypeName(current->value.vectorInfo.elementType));
                        break;
                    default:
                        printf("%-10s ", "UNKNOWN");
                        printf("%-20s ", "");
                }
                
                printf("%-8d %-8d %-8s\n", current->line, current->column, 
                       current->isConstant ? "YES" : "NO");
                current = current->next;
            }
        }
    }
    
    printf("\nTotal symbols: %d\n", symTab->size);
}

// Free the symbol table and all its entries
void freeSymbolTable(SymbolTable* symTab) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        SymbolEntry* current = symTab->hashTable[i];
        
        while (current != NULL) {
            SymbolEntry* next = current->next;
            
            // Free allocated memory
            free(current->name);
            if (current->type == TYPE_STRING && current->value.stringValue != NULL) {
                free(current->value.stringValue);
            }
            
            free(current);
            current = next;
        }
    }
    
    free(symTab);
}

// Add this helper function
const char* getTypeName(SymbolType type) {
    switch (type) {
        case TYPE_INTEGER:
            return "INTEGER";
        case TYPE_FLOAT:
            return "FLOAT";
        case TYPE_CHAR:
            return "CHAR";
        case TYPE_STRING:
            return "STRING";
        case TYPE_BOOLEAN:
            return "BOOLEAN";
        case TYPE_CONST:
            return "CONST";
        case TYPE_VECTOR:
            return "VECTOR";
        default:
            return "UNKNOWN";
    }
}

// Add this function to get integer value from symbol table
int getIntValue(SymbolTable* symTab, const char* name) {
    SymbolEntry* entry = lookupSymbol(symTab, name);
    if (!entry) {
        fprintf(stderr, "Warning: Undefined variable '%s' used in expression\n", name);
        return 0; // Default for undefined variables
    }
    
    if (!entry->isInitialized) {
        fprintf(stderr, "Warning: Uninitialized variable '%s' used in expression\n", name);
        return 0;
    }
    
    switch (entry->type) {
        case TYPE_INTEGER:
            return entry->value.intValue;
        case TYPE_FLOAT:
            return (int)entry->value.floatValue;
        case TYPE_CHAR:
            return (int)entry->value.charValue;
        case TYPE_CONST:
            return entry->value.intValue;  // Assuming integer constants
        default:
            fprintf(stderr, "Warning: Variable '%s' is not a numeric type\n", name);
            return 0;
    }
}

// Convert any value to integer
int convertToInt(struct ExprValue expr) {
    switch (expr.type) {
        case TYPE_INTEGER: return expr.value.intValue;
        case TYPE_FLOAT:   return (int)expr.value.floatValue;
        case TYPE_CHAR:    return (int)expr.value.charValue;
        default:           return 0;
    }
}

// Convert any value to float
float convertToFloat(struct ExprValue expr) {
    switch (expr.type) {
        case TYPE_INTEGER: return (float)expr.value.intValue;
        case TYPE_FLOAT:   return expr.value.floatValue;
        case TYPE_CHAR:    return (float)expr.value.charValue;
        default:           return 0.0;
    }
}

// Check if two types are compatible for operations
int areTypesCompatible(SymbolType type1, SymbolType type2, char operation) {
    // String can only be used with + (concatenation) or = (assignment)
    if ((type1 == TYPE_STRING || type2 == TYPE_STRING)) {
        // For strings, allow + (concatenation) and = (assignment)
        if (operation == '+') {
            return 1;
        } else if (operation == '=') {
            // For assignment, if target is string, source must also be string
            return (type1 == TYPE_STRING && type2 == TYPE_STRING);
        } else {
            return 0; // Other operations not allowed with strings
        }
    }
    
    // Constants can't be on the left side of an assignment
    if (type1 == TYPE_CONST && operation == '=') {
        return 0;
    }
    
    // Vectors need special handling
    if (type1 == TYPE_VECTOR || type2 == TYPE_VECTOR) {
        // Vector operations depend on the operation
        return (operation == '+' || operation == '-' || operation == '*');
    }
    
    
    return 1;
} 