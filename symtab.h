#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expr_value.h"  // Include the ExprValue definition

// Define the SymbolType enum
typedef enum {
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_CONST,
    TYPE_VECTOR
} SymbolType;

// Define the table size
#define TABLE_SIZE 26  // One entry per letter (A-Z)

// Symbol table entry structure
typedef struct SymbolEntry {
    char* name;                  // Symbol name
    SymbolType type;            // Symbol type
    int line;                   // Line number where declared
    int column;                 // Column number where declared
    
    // Value depending on type
    union {
        int intValue;
        float floatValue;
        char charValue;
        char* stringValue;
        struct {
            int rows;
            int cols;
            SymbolType elementType;
        } vectorInfo;
    } value;
    
    int isInitialized;          // Whether value has been set
    int isConstant;             // Whether symbol is constant
    int isUsed;                 // Track if the symbol is used
    
    struct SymbolEntry* next;   // For linked list in hash table
} SymbolEntry;

// Symbol table structure
typedef struct {
    SymbolEntry* hashTable[TABLE_SIZE];
    int size;                   // Number of symbols in table
} SymbolTable;

// Global type tracking
extern SymbolType currentType;

// Function declarations
SymbolTable* createSymbolTable();
unsigned int hash(const char* name);
int insertSymbol(SymbolTable* symTab, const char* name, SymbolType type, int line, int column);
SymbolEntry* lookupSymbol(SymbolTable* symTab, const char* name);
void printSymbolTable(SymbolTable* symTab);
void freeSymbolTable(SymbolTable* symTab);
int getIntValue(SymbolTable* symTab, const char* name);

// Function declarations for type conversion
int convertToInt(struct ExprValue expr);
float convertToFloat(struct ExprValue expr);
int areTypesCompatible(SymbolType type1, SymbolType type2, char operation);

#endif /* SYMTAB_H */ 