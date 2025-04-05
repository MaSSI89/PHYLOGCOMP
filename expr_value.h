#ifndef EXPR_VALUE_H
#define EXPR_VALUE_H

// Do not include symtab.h to avoid circular dependency

struct ExprValue {
    int type;     // Type of the expression (using integer values)
    union {
        int intValue;
        float floatValue;
        char charValue;
        char* stringValue;
    } value;
    char valueStr[50]; // String representation of the value for quadruples
};

#endif // EXPR_VALUE_H 