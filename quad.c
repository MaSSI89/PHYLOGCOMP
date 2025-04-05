#include "quad.h"

// Static variables for temporary variable and label generation
int tempCount = 0;  // Counter for temporary variables (now global)
static int labelCount = 0; // Counter for labels

// Function to create a new quadruple list
QuadList* createQuadList() {
    QuadList* list = (QuadList*)malloc(sizeof(QuadList));
    if (!list) {
        fprintf(stderr, "Memory allocation failed for quadruple list\n");
        exit(1);
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    
    return list;
}

// Function to generate a new temporary variable name
char* generateTemp() {
    char* temp = (char*)malloc(10); // Enough space for "t" + number
    if (!temp) {
        fprintf(stderr, "Memory allocation failed for temporary variable\n");
        exit(1);
    }
    
    sprintf(temp, "t%d", tempCount++);
    return temp;
}

// Function to generate a new label
char* generateLabel() {
    char* label = (char*)malloc(10); // Enough space for "L" + number
    if (!label) {
        fprintf(stderr, "Memory allocation failed for label\n");
        exit(1);
    }
    
    sprintf(label, "L%d", labelCount++);
    return label;
}

// Function to add a quadruple to the list
void addQuad(QuadList* list, QuadOperation op, const char* arg1, const char* arg2, const char* result) {
    // Create a new quadruple
    Quadruple* quad = (Quadruple*)malloc(sizeof(Quadruple));
    if (!quad) {
        fprintf(stderr, "Memory allocation failed for quadruple\n");
        exit(1);
    }
    
    // Initialize the quadruple
    quad->op = op;
    quad->arg1 = arg1 ? strdup(arg1) : NULL;
    quad->arg2 = arg2 ? strdup(arg2) : NULL;
    quad->result = result ? strdup(result) : NULL;
    quad->next = NULL;
    
    // Add the quadruple to the list
    if (list->tail) {
        list->tail->next = quad;
        list->tail = quad;
    } else {
        list->head = quad;
        list->tail = quad;
    }
    
    list->count++;
}

// Function to generate code for binary operations
char* genBinaryOp(QuadList* list, QuadOperation op, const char* arg1, const char* arg2) {
    char* temp = generateTemp();
    addQuad(list, op, arg1, arg2, temp);
    return temp;
}

// Function to generate code for assignment
char* genAssignment(QuadList* list, const char* lvalue, const char* expr) {
    addQuad(list, QUAD_ASSIGN, expr, NULL, lvalue);
    return (char*)lvalue; // Return lvalue for chained assignments
}

// Function to generate a label
int genLabel(QuadList* list) {
    int labelNum = labelCount++;
    char label[20];
    sprintf(label, "L%d", labelNum);
    addQuad(list, QUAD_LABEL, NULL, NULL, label);
    return labelNum;
}

// Function to generate an unconditional jump
void genGoto(QuadList* list, int label) {
    char labelStr[20];
    sprintf(labelStr, "L%d", label);
    addQuad(list, QUAD_GOTO, NULL, NULL, labelStr);
}

// Function to generate a conditional jump
void genIfGoto(QuadList* list, QuadOperation op, const char* arg1, const char* arg2, int label) {
    char labelStr[20];
    sprintf(labelStr, "L%d", label);
    addQuad(list, op, arg1, arg2, labelStr);
}

// Function to generate a read quadruple
void genReadQuad(QuadList* list, const char* prompt, const char* var) {
    addQuad(list, QUAD_READ, prompt, NULL, var);
}

// Function to generate a write quadruple
void genWriteQuad(QuadList* list, const char* prompt, const char* var) {
    addQuad(list, QUAD_WRITE, prompt, var, NULL);
}

// Function to get the string representation of a quad operation
const char* getQuadOpName(QuadOperation op) {
    switch (op) {
        case QUAD_ADD: return "+";
        case QUAD_SUB: return "-";
        case QUAD_MULT: return "*";
        case QUAD_DIV: return "/";
        case QUAD_ASSIGN: return "=";
        case QUAD_LABEL: return "LABEL";
        case QUAD_GOTO: return "GOTO";
        case QUAD_IF_EQ: return "IF_EQ";
        case QUAD_IF_NE: return "IF_NE";
        case QUAD_IF_LT: return "IF_LT";
        case QUAD_IF_LE: return "IF_LE";
        case QUAD_IF_GT: return "IF_GT";
        case QUAD_IF_GE: return "IF_GE";
        case QUAD_IF_FALSE: return "IF_FALSE";
        case QUAD_READ: return "READ";
        case QUAD_WRITE: return "WRITE";
        default: return "UNKNOWN";
    }
}

// Function to print a quadruple list
void printQuadList(QuadList* list) {
    printf("\n=== Generated Quadruples ===\n");
    printf("No. | Operation | Arg1 | Arg2 | Result\n");
    printf("-------------------------------------\n");
    
    int i = 0;
    Quadruple* current = list->head;
    
    while (current) {
        // Format the arguments properly - especially for string literals
        char arg1Str[50] = "-";
        char arg2Str[50] = "-";
        char resultStr[50] = "-";
        
        if (current->arg1) {
            strncpy(arg1Str, current->arg1, sizeof(arg1Str) - 1);
            arg1Str[sizeof(arg1Str) - 1] = '\0';
        }
        
        if (current->arg2) {
            strncpy(arg2Str, current->arg2, sizeof(arg2Str) - 1);
            arg2Str[sizeof(arg2Str) - 1] = '\0';
        }
        
        if (current->result) {
            strncpy(resultStr, current->result, sizeof(resultStr) - 1);
            resultStr[sizeof(resultStr) - 1] = '\0';
        }
        
        printf("%3d | %-9s | %-6s | %-6s | %-6s\n", 
               i++, 
               getQuadOpName(current->op), 
               arg1Str, 
               arg2Str, 
               resultStr);
        current = current->next;
    }
    
    printf("-------------------------------------\n");
    printf("Total quadruples: %d\n", list->count);
}

// Function to free the quadruple list
void freeQuadList(QuadList* list) {
    Quadruple* current = list->head;
    Quadruple* next;
    
    while (current) {
        next = current->next;
        
        // Free the strings
        if (current->arg1) free(current->arg1);
        if (current->arg2) free(current->arg2);
        if (current->result) free(current->result);
        
        // Free the quadruple
        free(current);
        
        current = next;
    }
    
    // Free the list
    free(list);
} 