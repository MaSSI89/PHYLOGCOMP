#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "expr_value.h"

// Simple test program to demonstrate float constant support

int main() {
    printf("Testing float constant support\n");
    
    // Initialize symbol table
    SymbolTable* symtab = createSymbolTable();
    
    // Insert float constants
    insertSymbol(symtab, "pi", TYPE_FLOAT, 1, 1);
    insertSymbol(symtab, "e", TYPE_FLOAT, 2, 1);
    
    // Look up the symbols and set their values
    SymbolEntry* pi = lookupSymbol(symtab, "pi");
    SymbolEntry* e = lookupSymbol(symtab, "e");
    
    if (pi) {
        pi->isConstant = 1;
        pi->isInitialized = 1;
        pi->value.floatValue = 3.14159;
        printf("Set constant pi = %f\n", pi->value.floatValue);
    }
    
    if (e) {
        e->isConstant = 1;
        e->isInitialized = 1;
        e->value.floatValue = 2.71828;
        printf("Set constant e = %f\n", e->value.floatValue);
    }
    
    // Print symbol table
    printSymbolTable(symtab);
    
    // Clean up
    freeSymbolTable(symtab);
    
    return 0;
} 