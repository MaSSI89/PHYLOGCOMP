/*
 * optimizer.c - Code optimization module for compiler
 * 
 * Implements the following optimization techniques:
 * 1. Copy propagation
 * 2. Expression propagation 
 * 3. Common subexpression elimination
 * 4. Algebraic simplification
 * 5. Dead code elimination
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "optimizer.h"
#include "quad.h"

#define MAX_VARIABLES 1000
#define MAX_EXPRESSIONS 1000
#define MAX_QUADRUPLES 10000

typedef struct {
    char* name;           // Variable name or temp identifier
    char* expression;     // Expression assigned to this variable
    bool is_active;       // Whether this variable is still used
    int last_modified;    // Instruction number where value was last changed
    int last_used;        // Instruction number where variable was last used
} VariableInfo;

typedef struct {
    char* expression;     // Expression string (e.g., "4*j")
    char* result;         // Result temp (e.g., "t6")
    int instruction_num;  // Where this computation happened
} Expression;

// Global variables
static VariableInfo variables[MAX_VARIABLES];
static int var_count = 0;
static Expression expressions[MAX_EXPRESSIONS];
static int expr_count = 0;
static bool quadruple_active[MAX_QUADRUPLES]; // Track if quadruple is still needed

// Get a variable's current info, or create a new entry
static int get_variable_index(const char* name) {
    if (name == NULL) return -1;
    
    for (int i = 0; i < var_count; i++) {
        if (variables[i].name != NULL && strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    
    // Create new variable entry
    variables[var_count].name = strdup(name);
    variables[var_count].expression = NULL;
    variables[var_count].is_active = true;
    variables[var_count].last_modified = -1;
    variables[var_count].last_used = -1;
    
    return var_count++;
}

// Create a string representation of an expression
static char* create_expression_string(const char* op, const char* arg1, const char* arg2) {
    if (arg1 == NULL) return NULL;
    
    // For unary operations
    if (arg2 == NULL || strlen(arg2) == 0) {
        char* result = malloc(strlen(op) + strlen(arg1) + 3);
        sprintf(result, "%s%s", op, arg1);
        return result;
    }
    
    // For binary operations
    char* result = malloc(strlen(arg1) + strlen(op) + strlen(arg2) + 3);
    sprintf(result, "%s%s%s", arg1, op, arg2);
    return result;
}

// Find if an expression has already been computed
static int find_expression(const char* expr) {
    if (expr == NULL) return -1;
    
    for (int i = 0; i < expr_count; i++) {
        if (expressions[i].expression != NULL && 
            strcmp(expressions[i].expression, expr) == 0) {
            return i;
        }
    }
    return -1;
}

// Add a new computed expression
static void add_expression(const char* expr, const char* result, int instr_num) {
    if (expr == NULL || result == NULL || expr_count >= MAX_EXPRESSIONS) return;
    
    expressions[expr_count].expression = strdup(expr);
    expressions[expr_count].result = strdup(result);
    expressions[expr_count].instruction_num = instr_num;
    expr_count++;
}

// Perform algebraic simplifications on a quadruple
static bool simplify_algebra(Quadruple* quad) {
    if (quad == NULL) return false;
    
    bool changed = false;
    const char* op = getQuadOpName(quad->op);
    
    // Simplification: x = y + 0 or x = 0 + y => x = y
    if (quad->op == QUAD_ADD) {
        if (quad->arg1 && quad->arg2) {
            if (strcmp(quad->arg1, "0") == 0) {
                free(quad->arg1);
                quad->arg1 = strdup(quad->arg2);
                quad->op = QUAD_ASSIGN;
                free(quad->arg2);
                quad->arg2 = NULL;
                changed = true;
            } else if (strcmp(quad->arg2, "0") == 0) {
                quad->op = QUAD_ASSIGN;
                free(quad->arg2);
                quad->arg2 = NULL;
                changed = true;
            }
        }
    }
    
    // Simplification: x = y - 0 => x = y
    if (quad->op == QUAD_SUB && quad->arg2 && strcmp(quad->arg2, "0") == 0) {
        quad->op = QUAD_ASSIGN;
        free(quad->arg2);
        quad->arg2 = NULL;
        changed = true;
    }
    
    // Simplification: x = y * 1 or x = 1 * y => x = y
    if (quad->op == QUAD_MULT && quad->arg1 && quad->arg2) {
        if (strcmp(quad->arg1, "1") == 0) {
            free(quad->arg1);
            quad->arg1 = strdup(quad->arg2);
            quad->op = QUAD_ASSIGN;
            free(quad->arg2);
            quad->arg2 = NULL;
            changed = true;
        } else if (strcmp(quad->arg2, "1") == 0) {
            quad->op = QUAD_ASSIGN;
            free(quad->arg2);
            quad->arg2 = NULL;
            changed = true;
        }
        // Optimization: x = y * 2 => x = y + y
        else if (strcmp(quad->arg2, "2") == 0) {
            quad->op = QUAD_ADD;
            free(quad->arg2);
            quad->arg2 = strdup(quad->arg1);
            changed = true;
        }
    }
    
    // Simplification: x = y / 1 => x = y
    if (quad->op == QUAD_DIV && quad->arg2 && strcmp(quad->arg2, "1") == 0) {
        quad->op = QUAD_ASSIGN;
        free(quad->arg2);
        quad->arg2 = NULL;
        changed = true;
    }
    
    return changed;
}

// Main optimization function
void optimize_code(QuadList* quad_list) {
    if (!quad_list) return;
    
    int quad_count = quad_list->count;
    bool changes_made;
    int pass = 0;
    
    // Initialize all quadruples as active
    for (int i = 0; i < quad_count && i < MAX_QUADRUPLES; i++) {
        quadruple_active[i] = true;
    }
    
    // Multiple passes may be needed until no more optimizations can be applied
    do {
        changes_made = false;
        pass++;
        printf("Starting optimization pass %d\n", pass);
        
        // First pass - analyze and populate data structures
        int i = 0;
        Quadruple* current = quad_list->head;
        
        while (current) {
            if (!quadruple_active[i]) {
                current = current->next;
                i++;
                continue;
            }
            
            // Record variable usage for later dead code elimination
            if (current->arg1 && strlen(current->arg1) > 0) {
                int var_idx = get_variable_index(current->arg1);
                if (var_idx >= 0) {
                    variables[var_idx].last_used = i;
                }
            }
            
            if (current->arg2 && strlen(current->arg2) > 0) {
                int var_idx = get_variable_index(current->arg2);
                if (var_idx >= 0) {
                    variables[var_idx].last_used = i;
                }
            }
            
            if (current->result && strlen(current->result) > 0) {
                int var_idx = get_variable_index(current->result);
                if (var_idx >= 0) {
                    variables[var_idx].last_modified = i;
                    
                    // If this is an assignment
                    if (current->op == QUAD_ASSIGN && current->arg1) {
                        variables[var_idx].expression = strdup(current->arg1);
                    }
                    // Otherwise, it's a computation
                    else if (current->arg1 && (current->op == QUAD_ADD || 
                             current->op == QUAD_SUB || 
                             current->op == QUAD_MULT || 
                             current->op == QUAD_DIV)) {
                        char* expr = create_expression_string(getQuadOpName(current->op), 
                                                            current->arg1, 
                                                            current->arg2);
                        if (expr) {
                            variables[var_idx].expression = expr;
                            
                            // Record for common subexpression elimination
                            add_expression(expr, current->result, i);
                        }
                    }
                }
            }
            
            current = current->next;
            i++;
        }
        
        // Second pass - apply optimizations
        i = 0;
        current = quad_list->head;
        
        while (current) {
            if (!quadruple_active[i]) {
                current = current->next;
                i++;
                continue;
            }
            
            // 1. Algebraic simplification
            if (simplify_algebra(current)) {
                changes_made = true;
                current = current->next;
                i++;
                continue;  // Move to next quadruple after simplification
            }
            
            // 2. Copy Propagation - replace variable references with their values
            if (current->arg1 && strlen(current->arg1) > 0) {
                int var_idx = get_variable_index(current->arg1);
                if (var_idx >= 0 && variables[var_idx].expression != NULL && 
                    variables[var_idx].is_active &&
                    current->op == QUAD_ASSIGN) {
                    
                    // Simple variable to variable copy
                    free(current->arg1);
                    current->arg1 = strdup(variables[var_idx].expression);
                    changes_made = true;
                }
            }
            
            if (current->arg2 && strlen(current->arg2) > 0) {
                int var_idx = get_variable_index(current->arg2);
                if (var_idx >= 0 && variables[var_idx].expression != NULL && 
                    variables[var_idx].is_active) {
                    
                    // Simple variable to variable copy
                    free(current->arg2);
                    current->arg2 = strdup(variables[var_idx].expression);
                    changes_made = true;
                }
            }
            
            // 3. Common Subexpression Elimination
            if (current->op != QUAD_ASSIGN && current->arg1 && current->arg2 && 
                (current->op == QUAD_ADD || current->op == QUAD_SUB || 
                 current->op == QUAD_MULT || current->op == QUAD_DIV)) {
                
                char* expr = create_expression_string(getQuadOpName(current->op), 
                                                    current->arg1, 
                                                    current->arg2);
                if (expr) {
                    int expr_idx = find_expression(expr);
                    
                    if (expr_idx >= 0 && expressions[expr_idx].instruction_num != i) {
                        // This expression was computed before
                        // Replace with a simple copy
                        current->op = QUAD_ASSIGN;
                        free(current->arg1);
                        current->arg1 = strdup(expressions[expr_idx].result);
                        free(current->arg2);
                        current->arg2 = NULL;
                        
                        changes_made = true;
                    }
                    else {
                        // This is a new expression, record it
                        add_expression(expr, current->result, i);
                    }
                    free(expr);
                }
            }
            
            current = current->next;
            i++;
        }
        
        // 4. Dead Code Elimination
        i = 0;
        current = quad_list->head;
        
        while (current) {
            if (!quadruple_active[i]) {
                current = current->next;
                i++;
                continue;
            }
            
            // Skip if this is not an assignment or computation
            if (!current->result || strlen(current->result) == 0) {
                current = current->next;
                i++;
                continue;
            }
            
            // Skip special quad types that we don't want to eliminate
            if (current->op == QUAD_LABEL || current->op == QUAD_GOTO || 
                current->op == QUAD_IF_FALSE || current->op == QUAD_READ ||
                current->op == QUAD_WRITE) {
                current = current->next;
                i++;
                continue;
            }
                
            int var_idx = get_variable_index(current->result);
            if (var_idx >= 0) {
                // If this variable is never used after definition, mark as dead code
                if (variables[var_idx].last_used < variables[var_idx].last_modified) {
                    // Don't eliminate if it might have side effects or is a special name
                    if (current->result[0] != 'L' && // Not a label
                        strstr(current->result, "[") == NULL) { // Not an array access
                        
                        quadruple_active[i] = false;
                        variables[var_idx].is_active = false;
                        changes_made = true;
                        printf("Eliminating dead code at instruction %d: %s = %s %s %s\n", 
                               i, current->result, current->arg1 ? current->arg1 : "", 
                               getQuadOpName(current->op), current->arg2 ? current->arg2 : "");
                    }
                }
            }
            
            current = current->next;
            i++;
        }
        
    } while (changes_made && pass < 5); // Limit number of passes to prevent infinite loops
    
    // Final pass - mark inactive quadruples as NOPs
    int removed = 0;
    int i = 0;
    Quadruple* current = quad_list->head;
    
    while (current) {
        if (!quadruple_active[i]) {
            // Mark as a NOP by setting all fields to indicate "no operation"
            free(current->arg1); current->arg1 = strdup("NOP");
            free(current->arg2); current->arg2 = NULL;
            free(current->result); current->result = strdup("NOP");
            // Use a special operation code that will be ignored
            current->op = QUAD_LABEL; // Reuse label op as it's harmless if args are NOP
            removed++;
        }
        
        current = current->next;
        i++;
    }
    
    printf("Optimization complete: Eliminated %d quadruples out of %d\n", removed, quad_count);
    
    // Clean up
    for (int i = 0; i < var_count; i++) {
        free(variables[i].name);
        if (variables[i].expression) free(variables[i].expression);
    }
    
    for (int i = 0; i < expr_count; i++) {
        free(expressions[i].expression);
        free(expressions[i].result);
    }
    
    // Reset counters for next optimization
    var_count = 0;
    expr_count = 0;
}

// Print the optimized quadruples
void print_optimized_code(QuadList* quad_list) {
    if (!quad_list) return;
    
    printf("\n--- Optimized Code (Only Active Quadruples) ---\n");
    printf("No. | Operation | Arg1 | Arg2 | Result\n");
    printf("-------------------------------------\n");
    
    int i = 0;
    int active_count = 0;
    Quadruple* current = quad_list->head;
    
    while (current) {
        // Skip NOPs from optimization
        if (quadruple_active[i] && 
            !(current->op == QUAD_LABEL && 
              current->arg1 && strcmp(current->arg1, "NOP") == 0 &&
              current->result && strcmp(current->result, "NOP") == 0)) {
            
            active_count++;
            
            // Format the output
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
                   i, getQuadOpName(current->op), arg1Str, arg2Str, resultStr);
        }
        
        current = current->next;
        i++;
    }
    
    printf("-------------------------------------\n");
    printf("Total active quadruples: %d (Eliminated: %d)\n", active_count, quad_list->count - active_count);
}