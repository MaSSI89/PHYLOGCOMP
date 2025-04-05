#ifndef QUAD_H
#define QUAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// Define quadruple operations
typedef enum {
    QUAD_ASSIGN,   // =
    QUAD_ADD,      // +
    QUAD_SUB,      // -
    QUAD_MULT,     // *
    QUAD_DIV,      // /
    QUAD_IF_GT,    // if >
    QUAD_IF_LT,    // if <
    QUAD_IF_GE,    // if >=
    QUAD_IF_LE,    // if <=
    QUAD_IF_EQ,    // if ==
    QUAD_IF_NE,    // if !=
    QUAD_IF_FALSE, // if false jump to label
    QUAD_GOTO,     // unconditional jump
    QUAD_LABEL,    // label definition
    // QUAD_FUNCTION_START, // function start
    // QUAD_FUNCTION_END,   // function end
    // QUAD_FUNCTION_CALL,  // function call
    // QUAD_FUNCTION_RET,   // function return
    // QUAD_FUNCTION_PARAM, // function parameter
    // QUAD_FUNCTION_LOCAL, // function local variable
    QUAD_READ,     // read operation
    QUAD_WRITE     // write operation
} QuadOperation;

// Quadruple structure
typedef struct Quadruple {
    QuadOperation op;    // Operation
    char* arg1;          // First argument
    char* arg2;          // Second argument
    char* result;        // Result
    struct Quadruple* next; // For linked list implementation
} Quadruple;

// Quadruple list structure
typedef struct {
    Quadruple* head;     // First quadruple
    Quadruple* tail;     // Last quadruple
    int count;           // Number of quadruples
} QuadList;

// Make temporary variable counter visible externally
extern int tempCount;

// Function to create and initialize a new quadruple list
QuadList* createQuadList();

// Function to generate a new temporary variable name
char* generateTemp();

// Function to generate a new label
char* generateLabel();

// Function to add a quadruple to the list
void addQuad(QuadList* list, QuadOperation op, const char* arg1, const char* arg2, const char* result);

// Functions for specific operations
char* genBinaryOp(QuadList* list, QuadOperation op, const char* arg1, const char* arg2);
char* genAssignment(QuadList* list, const char* lvalue, const char* expr);
int genLabel(QuadList* list);
void genGoto(QuadList* list, int label);
void genIfGoto(QuadList* list, QuadOperation op, const char* arg1, const char* arg2, int label);
void genReadQuad(QuadList* list, const char* prompt, const char* var);
void genWriteQuad(QuadList* list, const char* prompt, const char* var);

// Function to print the quadruple list
void printQuadList(QuadList* list);

// Function to free the quadruple list
void freeQuadList(QuadList* list);

// Function to get a string representation of a quad operation
const char* getQuadOpName(QuadOperation op);

#endif /* QUAD_H */ 