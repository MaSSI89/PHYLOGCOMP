/*
 * optimizer.c - Advanced code optimization module for compiler
 * 
 * Implements the following optimization techniques:
 * 1. Copy propagation
 * 2. Constant propagation
 * 3. Constant folding 
 * 4. Common subexpression elimination
 * 5. Algebraic simplification
 * 6. Dead code elimination
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "optimizer.h"
#include "quad.h"

#define MAX_VARIABLES 1000
#define MAX_EXPRESSIONS 1000
#define MAX_QUADRUPLES 10000
#define MAX_PASSES 10

/* 
 * Improved variable information structure that includes constant value tracking
 * and dependency information
 */
typedef struct {
    char* name;                 // Variable name or temp identifier
    char* expression;           // Expression assigned to this variable
    bool is_active;             // Whether this variable is still used
    bool is_constant;           // Whether this variable holds a constant value
    bool is_initialized;        // Whether this variable is initialized
    char* constant_value;       // String representation of constant value (if constant)
    int last_modified;          // Instruction number where value was last changed
    int last_used;              // Instruction number where variable was last used
    int dependency_count;       // Number of variables that depend on this one
    int* dependencies;          // Array of variable indices that depend on this one
} VariableInfo;

/* 
 * Improved expression structure with additional type info
 */
typedef struct {
    char* expression;           // Expression string (e.g., "4*j")
    char* result;               // Result temp (e.g., "t6")
    int instruction_num;        // Where this computation happened
    QuadOperation op;           // Operation type
} Expression;

// Global variables
static VariableInfo variables[MAX_VARIABLES];
static int var_count = 0;
static Expression expressions[MAX_EXPRESSIONS];
static int expr_count = 0;
static bool quadruple_active[MAX_QUADRUPLES]; // Track if quadruple is still needed

// Optimization statistics
static OptimizationStats opt_stats;

// Function prototypes
static void init_optimization_stats();
static void reset_optimizer();
static bool is_constant(const char* str);
static char* eval_constant_expr(const char* op, const char* arg1, const char* arg2);
static int get_variable_index(const char* name);
static void add_dependency(int var_idx, int depends_on_idx);
static char* create_expression_string(const char* op, const char* arg1, const char* arg2);
static int find_expression(const char* expr, QuadOperation op);
static void add_expression(const char* expr, const char* result, int instr_num, QuadOperation op);
static bool simplify_algebra(Quadruple* quad);
static bool fold_constants(Quadruple* quad);
static bool propagate_constants(Quadruple* quad);
static void analyze_variables(QuadList* quad_list);
static bool propagate_copies(Quadruple* quad);
static bool eliminate_common_subexpressions(Quadruple* quad, int quad_index);
static bool eliminate_dead_code(QuadList* quad_list);
static bool apply_optimizations(QuadList* quad_list, OptimizationFlags flags);
static void count_active_quadruples(QuadList* quad_list);

// Initialize optimization statistics
static void init_optimization_stats() {
    memset(&opt_stats, 0, sizeof(OptimizationStats));
}

// Reset variables for a new optimization
static void reset_optimizer() {
    // Free all allocated memory
    for (int i = 0; i < var_count; i++) {
        free(variables[i].name);
        if (variables[i].expression) free(variables[i].expression);
        if (variables[i].constant_value) free(variables[i].constant_value);
        if (variables[i].dependencies) free(variables[i].dependencies);
    }
    
    for (int i = 0; i < expr_count; i++) {
        if (expressions[i].expression) free(expressions[i].expression);
        if (expressions[i].result) free(expressions[i].result);
    }
    
    // Reset counters
    var_count = 0;
    expr_count = 0;
}

// Check if a string represents a numeric constant
static bool is_constant(const char* str) {
    if (!str || !*str) return false;
    
    // Check for negative sign
    int start = 0;
    if (str[0] == '-') start = 1;
    
    // Check if all remaining characters are digits
    for (int i = start; str[i]; i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return false;
        }
    }
    
    return true;
}

// Evaluate a binary operation on constants
static char* eval_constant_expr(const char* op, const char* arg1, const char* arg2) {
    if (!op || !arg1) return NULL;
    
    // For unary operations, arg2 may be NULL
    if (!arg2 && strcmp(op, "=") != 0) return NULL;
    
    // Make sure both arguments are constants
    if (!is_constant(arg1) || (arg2 && !is_constant(arg2))) return NULL;
    
    double val1 = atof(arg1);
    double val2 = arg2 ? atof(arg2) : 0.0;
    double result = 0.0;
    char* result_str = malloc(32); // Enough space for most numbers
    
    if (strcmp(op, "+") == 0) {
        result = val1 + val2;
    } else if (strcmp(op, "-") == 0) {
        result = val1 - val2;
    } else if (strcmp(op, "*") == 0) {
        result = val1 * val2;
    } else if (strcmp(op, "/") == 0) {
        if (val2 == 0.0) return NULL; // Avoid division by zero
        result = val1 / val2;
    } else if (strcmp(op, "=") == 0) {
        result = val1;
    } else {
        free(result_str);
        return NULL;
    }
    
    // Check if result is an integer
    if (result == (int)result) {
        sprintf(result_str, "%d", (int)result);
    } else {
        sprintf(result_str, "%f", result);
    }
    
    return result_str;
}

// Get a variable's current info, or create a new entry
static int get_variable_index(const char* name) {
    if (name == NULL) return -1;
    
    for (int i = 0; i < var_count; i++) {
        if (variables[i].name != NULL && strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    
    // Create new variable entry
    if (var_count >= MAX_VARIABLES) {
        fprintf(stderr, "Warning: Maximum number of variables exceeded\n");
        return -1;
    }
    
    variables[var_count].name = strdup(name);
    variables[var_count].expression = NULL;
    variables[var_count].is_active = true;
    variables[var_count].is_constant = false;
    variables[var_count].is_initialized = false;
    variables[var_count].constant_value = NULL;
    variables[var_count].last_modified = -1;
    variables[var_count].last_used = -1;
    variables[var_count].dependency_count = 0;
    variables[var_count].dependencies = malloc(MAX_VARIABLES * sizeof(int)); // Pre-allocate dependencies array
    
    return var_count++;
}

// Add a dependency between variables
static void add_dependency(int var_idx, int depends_on_idx) {
    if (var_idx < 0 || var_idx >= var_count || depends_on_idx < 0 || depends_on_idx >= var_count) {
        return;
    }
    
    // Check if dependency already exists
    for (int i = 0; i < variables[var_idx].dependency_count; i++) {
        if (variables[var_idx].dependencies[i] == depends_on_idx) {
            return;
        }
    }
    
    // Add the dependency
    variables[var_idx].dependencies[variables[var_idx].dependency_count++] = depends_on_idx;
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
static int find_expression(const char* expr, QuadOperation op) {
    if (expr == NULL) return -1;
    
    for (int i = 0; i < expr_count; i++) {
        if (expressions[i].expression != NULL && 
            expressions[i].op == op &&
            strcmp(expressions[i].expression, expr) == 0) {
            return i;
        }
    }
    return -1;
}

// Add a new computed expression
static void add_expression(const char* expr, const char* result, int instr_num, QuadOperation op) {
    if (expr == NULL || result == NULL || expr_count >= MAX_EXPRESSIONS) return;
    
    expressions[expr_count].expression = strdup(expr);
    expressions[expr_count].result = strdup(result);
    expressions[expr_count].instruction_num = instr_num;
    expressions[expr_count].op = op;
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
                opt_stats.algebraic_simp++;
            } else if (strcmp(quad->arg2, "0") == 0) {
                quad->op = QUAD_ASSIGN;
                free(quad->arg2);
                quad->arg2 = NULL;
                changed = true;
                opt_stats.algebraic_simp++;
            }
        }
    }
    
    // Simplification: x = y - 0 => x = y
    else if (quad->op == QUAD_SUB && quad->arg2 && strcmp(quad->arg2, "0") == 0) {
        quad->op = QUAD_ASSIGN;
        free(quad->arg2);
        quad->arg2 = NULL;
        changed = true;
        opt_stats.algebraic_simp++;
    }
    
    // Simplification: x = y * 1 or x = 1 * y => x = y
    else if (quad->op == QUAD_MULT && quad->arg1 && quad->arg2) {
        if (strcmp(quad->arg1, "1") == 0) {
            free(quad->arg1);
            quad->arg1 = strdup(quad->arg2);
            quad->op = QUAD_ASSIGN;
            free(quad->arg2);
            quad->arg2 = NULL;
            changed = true;
            opt_stats.algebraic_simp++;
        } else if (strcmp(quad->arg2, "1") == 0) {
            quad->op = QUAD_ASSIGN;
            free(quad->arg2);
            quad->arg2 = NULL;
            changed = true;
            opt_stats.algebraic_simp++;
        }
        // Optimization: x = y * 2 => x = y + y
        else if (strcmp(quad->arg2, "2") == 0) {
            quad->op = QUAD_ADD;
            free(quad->arg2);
            quad->arg2 = strdup(quad->arg1);
            changed = true;
            opt_stats.algebraic_simp++;
        }
        // Optimization: x = y * 0 => x = 0
        else if (strcmp(quad->arg1, "0") == 0 || strcmp(quad->arg2, "0") == 0) {
            quad->op = QUAD_ASSIGN;
            free(quad->arg1);
            quad->arg1 = strdup("0");
            free(quad->arg2);
            quad->arg2 = NULL;
            changed = true;
            opt_stats.algebraic_simp++;
        }
    }
    
    // Simplification: x = y / 1 => x = y
    else if (quad->op == QUAD_DIV && quad->arg2 && strcmp(quad->arg2, "1") == 0) {
        quad->op = QUAD_ASSIGN;
        free(quad->arg2);
        quad->arg2 = NULL;
        changed = true;
        opt_stats.algebraic_simp++;
    }
    
    // Simplification: x = y / y => x = 1 (if y != 0)
    else if (quad->op == QUAD_DIV && quad->arg1 && quad->arg2 && 
             strcmp(quad->arg1, quad->arg2) == 0 && 
             strcmp(quad->arg1, "0") != 0) {
        quad->op = QUAD_ASSIGN;
        free(quad->arg1);
        quad->arg1 = strdup("1");
        free(quad->arg2);
        quad->arg2 = NULL;
        changed = true;
        opt_stats.algebraic_simp++;
    }
    
    return changed;
}

// Perform constant folding on a quadruple (evaluate operations on constants at compile time)
static bool fold_constants(Quadruple* quad) {
    if (quad == NULL) return false;
    
    // Skip if not an arithmetic operation
    if (quad->op != QUAD_ADD && quad->op != QUAD_SUB && 
        quad->op != QUAD_MULT && quad->op != QUAD_DIV) {
        return false;
    }
    
    // Check if both operands are constants
    if (!quad->arg1 || !quad->arg2 || !is_constant(quad->arg1) || !is_constant(quad->arg2)) {
        return false;
    }
    
    // Avoid division by zero
    if (quad->op == QUAD_DIV && strcmp(quad->arg2, "0") == 0) {
        return false;
    }
    
    // Evaluate the constant expression
    char* result = eval_constant_expr(getQuadOpName(quad->op), quad->arg1, quad->arg2);
    if (!result) return false;
    
    // Replace with a simple assignment of the constant
    quad->op = QUAD_ASSIGN;
    free(quad->arg1);
    quad->arg1 = result;
    free(quad->arg2);
    quad->arg2 = NULL;
    
    opt_stats.const_fold++;
    return true;
}

// Propagate constants (replace variables with their constant values)
static bool propagate_constants(Quadruple* quad) {
    if (quad == NULL) return false;
    
    bool changed = false;
    
    // Try to propagate constants in arg1
    if (quad->arg1 && strlen(quad->arg1) > 0 && !is_constant(quad->arg1)) {
        int var_idx = get_variable_index(quad->arg1);
        if (var_idx >= 0 && variables[var_idx].is_constant && variables[var_idx].constant_value) {
            free(quad->arg1);
            quad->arg1 = strdup(variables[var_idx].constant_value);
            changed = true;
            opt_stats.const_prop++;
        }
    }
    
    // Try to propagate constants in arg2
    if (quad->arg2 && strlen(quad->arg2) > 0 && !is_constant(quad->arg2)) {
        int var_idx = get_variable_index(quad->arg2);
        if (var_idx >= 0 && variables[var_idx].is_constant && variables[var_idx].constant_value) {
            free(quad->arg2);
            quad->arg2 = strdup(variables[var_idx].constant_value);
            changed = true;
            opt_stats.const_prop++;
        }
    }
    
    return changed;
}

// Main optimization function
void optimize_code(QuadList* quad_list) {
    // Default to all optimizations
    optimize_code_with_flags(quad_list, OPT_ALL);
}

// Optimize with specific flags
void optimize_code_with_flags(QuadList* quad_list, OptimizationFlags flags) {
    if (!quad_list) return;
    
    // Initialize statistics
    init_optimization_stats();
    opt_stats.total_quads = quad_list->count;
    
    // Reset optimizer state
    reset_optimizer();
    
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
        
        // First pass - analyze variables and build dependencies
        analyze_variables(quad_list);
        
        // Second pass - apply optimizations
        changes_made = apply_optimizations(quad_list, flags);
        
    } while (changes_made && pass < MAX_PASSES);
    
    opt_stats.optimization_passes = pass;
    
    // Final pass - count active quadruples
    count_active_quadruples(quad_list);
    
    printf("Optimization complete: Eliminated %d quadruples out of %d (%d passes)\n", 
           opt_stats.eliminated_quads, opt_stats.total_quads, pass);
    printf("  - Algebraic simplifications: %d\n", opt_stats.algebraic_simp);
    printf("  - Constant folding: %d\n", opt_stats.const_fold);
    printf("  - Constant propagation: %d\n", opt_stats.const_prop);
    printf("  - Copy propagation: %d\n", opt_stats.copy_prop);
    printf("  - Common subexpression elimination: %d\n", opt_stats.cse);
    printf("  - Dead code elimination: %d\n", opt_stats.dead_code);
}

// Analyze variables and build dependencies graph
static void analyze_variables(QuadList* quad_list) {
    if (!quad_list) return;
    
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
                variables[var_idx].is_initialized = true;
                
                // If variable is a constant, mark it
                if (is_constant(current->arg1)) {
                    variables[var_idx].is_constant = true;
                    variables[var_idx].constant_value = strdup(current->arg1);
                }
            }
        }
        
        if (current->arg2 && strlen(current->arg2) > 0) {
            int var_idx = get_variable_index(current->arg2);
            if (var_idx >= 0) {
                variables[var_idx].last_used = i;
                variables[var_idx].is_initialized = true;
                
                // If variable is a constant, mark it
                if (is_constant(current->arg2)) {
                    variables[var_idx].is_constant = true;
                    variables[var_idx].constant_value = strdup(current->arg2);
                }
            }
        }
        
        if (current->result && strlen(current->result) > 0) {
            int result_idx = get_variable_index(current->result);
            if (result_idx >= 0) {
                variables[result_idx].last_modified = i;
                
                // If this is an assignment
                if (current->op == QUAD_ASSIGN && current->arg1) {
                    variables[result_idx].expression = strdup(current->arg1);
                    
                    // Record dependency
                    int arg1_idx = get_variable_index(current->arg1);
                    if (arg1_idx >= 0) {
                        add_dependency(result_idx, arg1_idx);
                    }
                    
                    // Check if assigning a constant
                    if (is_constant(current->arg1)) {
                        variables[result_idx].is_constant = true;
                        variables[result_idx].constant_value = strdup(current->arg1);
                    }
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
                        variables[result_idx].expression = expr;
                        variables[result_idx].is_initialized = true;
                        
                        // Record dependencies
                        int arg1_idx = get_variable_index(current->arg1);
                        int arg2_idx = get_variable_index(current->arg2);
                        if (arg1_idx >= 0) add_dependency(result_idx, arg1_idx);
                        if (arg2_idx >= 0) add_dependency(result_idx, arg2_idx);
                        
                        // Record for common subexpression elimination
                        add_expression(expr, current->result, i, current->op);
                        
                        // Check if operation on constants
                        if (current->arg1 && current->arg2 && 
                            is_constant(current->arg1) && is_constant(current->arg2)) {
                            char* const_result = eval_constant_expr(
                                getQuadOpName(current->op), current->arg1, current->arg2);
                            if (const_result) {
                                variables[result_idx].is_constant = true;
                                variables[result_idx].constant_value = const_result;
                            }
                        }
                    }
                }
            }
        }
        
        current = current->next;
        i++;
    }
}

// Apply all applicable optimizations
static bool apply_optimizations(QuadList* quad_list, OptimizationFlags flags) {
    if (!quad_list) return false;
    
    bool changes_made = false;
    int i = 0;
    Quadruple* current = quad_list->head;
    
    while (current) {
        if (!quadruple_active[i]) {
            current = current->next;
            i++;
            continue;
        }
        
        // Apply algebraic simplifications
        if ((flags & OPT_ALGEBRAIC) && simplify_algebra(current)) {
            changes_made = true;
        }
        
        // Apply constant folding
        if ((flags & OPT_CONST_FOLDING) && fold_constants(current)) {
            changes_made = true;
        }
        
        // Apply constant propagation
        if ((flags & OPT_CONST_PROPAGATION) && propagate_constants(current)) {
            changes_made = true;
        }
        
        // Apply copy propagation
        if ((flags & OPT_COPY_PROPAGATION) && propagate_copies(current)) {
            changes_made = true;
        }
        
        // Apply common subexpression elimination
        if ((flags & OPT_CSE) && eliminate_common_subexpressions(current, i)) {
            changes_made = true;
        }
        
        current = current->next;
        i++;
    }
    
    // Apply dead code elimination as a separate pass
    if (flags & OPT_DEAD_CODE) {
        if (eliminate_dead_code(quad_list)) {
            changes_made = true;
        }
    }
    
    return changes_made;
}

// Apply copy propagation optimization
static bool propagate_copies(Quadruple* quad) {
    if (quad == NULL) return false;
    
    bool changed = false;
    
    // Copy propagation for arg1
    if (quad->arg1 && strlen(quad->arg1) > 0 && !is_constant(quad->arg1)) {
        int var_idx = get_variable_index(quad->arg1);
        if (var_idx >= 0 && variables[var_idx].expression != NULL && 
            variables[var_idx].is_active && 
            !variables[var_idx].is_constant) {  // don't propagate if it's a constant (handled separately)
            
            free(quad->arg1);
            quad->arg1 = strdup(variables[var_idx].expression);
            changed = true;
            opt_stats.copy_prop++;
        }
    }
    
    // Copy propagation for arg2
    if (quad->arg2 && strlen(quad->arg2) > 0 && !is_constant(quad->arg2)) {
        int var_idx = get_variable_index(quad->arg2);
        if (var_idx >= 0 && variables[var_idx].expression != NULL && 
            variables[var_idx].is_active &&
            !variables[var_idx].is_constant) {  // don't propagate if it's a constant
            
            free(quad->arg2);
            quad->arg2 = strdup(variables[var_idx].expression);
            changed = true;
            opt_stats.copy_prop++;
        }
    }
    
    return changed;
}

// Apply common subexpression elimination optimization
static bool eliminate_common_subexpressions(Quadruple* quad, int quad_index) {
    if (quad == NULL || quad->op == QUAD_ASSIGN) return false;
    
    // Skip if not an arithmetic operation
    if (quad->op != QUAD_ADD && quad->op != QUAD_SUB && 
        quad->op != QUAD_MULT && quad->op != QUAD_DIV) {
        return false;
    }
    
    // Check if both operands exist
    if (!quad->arg1 || !quad->arg2) return false;
    
    // Create expression string
    char* expr = create_expression_string(getQuadOpName(quad->op), quad->arg1, quad->arg2);
    if (!expr) return false;
    
    // Look for existing computation of this expression
    int expr_idx = find_expression(expr, quad->op);
    if (expr_idx >= 0 && expressions[expr_idx].instruction_num != quad_index) {
        // This expression was computed before
        // Replace with a simple copy
        quad->op = QUAD_ASSIGN;
        free(quad->arg1);
        quad->arg1 = strdup(expressions[expr_idx].result);
        free(quad->arg2);
        quad->arg2 = NULL;
        
        free(expr);
        opt_stats.cse++;
        return true;
    }
    
    // Record this as a new expression
    add_expression(expr, quad->result, quad_index, quad->op);
    free(expr);
    return false;
}

// Apply dead code elimination optimization
static bool eliminate_dead_code(QuadList* quad_list) {
    if (!quad_list) return false;
    
    bool changes_made = false;
    int i = 0;
    Quadruple* current = quad_list->head;
    
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
                    opt_stats.dead_code++;
                    
                    printf("Eliminating dead code at instruction %d: %s = %s %s %s\n", 
                           i, current->result, current->arg1 ? current->arg1 : "", 
                           getQuadOpName(current->op), current->arg2 ? current->arg2 : "");
                }
            }
        }
        
        current = current->next;
        i++;
    }
    
    return changes_made;
}

// Count active quadruples and update statistics
static void count_active_quadruples(QuadList* quad_list) {
    if (!quad_list) return;
    
    int active_count = 0;
    int i = 0;
    Quadruple* current = quad_list->head;
    
    while (current) {
        if (quadruple_active[i]) {
            active_count++;
        } else {
            // Mark as a NOP by setting all fields to indicate "no operation"
            free(current->arg1);
            current->arg1 = strdup("NOP");
            free(current->arg2);
            current->arg2 = NULL;
            free(current->result);
            current->result = strdup("NOP");
            current->op = QUAD_LABEL; // Reuse label op as it's harmless if args are NOP
        }
        
        current = current->next;
        i++;
    }
    
    opt_stats.active_quads = active_count;
    opt_stats.eliminated_quads = opt_stats.total_quads - active_count;
}

// Get the optimization statistics
OptimizationStats get_optimization_stats() {
    return opt_stats;
}

// Print the optimized code to stdout
void print_optimized_code(QuadList* quad_list) {
    if (!quad_list) return;
    
    printf("\n--- Optimized Code (Only Active Quadruples) ---\n");
    printf("No. | Operation | Arg1     | Arg2     | Result\n");
    printf("----------------------------------------------\n");
    
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
            char arg1Str[20] = "-";
            char arg2Str[20] = "-";
            char resultStr[20] = "-";
            
            if (current->arg1) {
                // Truncate if too long
                if (strlen(current->arg1) > 18) {
                    strncpy(arg1Str, current->arg1, 15);
                    strcat(arg1Str, "...");
                } else {
                    strncpy(arg1Str, current->arg1, sizeof(arg1Str) - 1);
                }
                arg1Str[sizeof(arg1Str) - 1] = '\0';
            }
            
            if (current->arg2) {
                // Truncate if too long
                if (strlen(current->arg2) > 18) {
                    strncpy(arg2Str, current->arg2, 15);
                    strcat(arg2Str, "...");
                } else {
                    strncpy(arg2Str, current->arg2, sizeof(arg2Str) - 1);
                }
                arg2Str[sizeof(arg2Str) - 1] = '\0';
            }
            
            if (current->result) {
                // Truncate if too long
                if (strlen(current->result) > 18) {
                    strncpy(resultStr, current->result, 15);
                    strcat(resultStr, "...");
                } else {
                    strncpy(resultStr, current->result, sizeof(resultStr) - 1);
                }
                resultStr[sizeof(resultStr) - 1] = '\0';
            }
            
            printf("%3d | %-9s | %-8s | %-8s | %-8s\n", 
                   i, getQuadOpName(current->op), arg1Str, arg2Str, resultStr);
        }
        
        current = current->next;
        i++;
    }
    
    printf("----------------------------------------------\n");
    printf("Total active quadruples: %d (Eliminated: %d)\n", active_count, quad_list->count - active_count);
    
    // Print optimization statistics
    printf("\n--- Optimization Statistics ---\n");
    printf("Total passes: %d\n", opt_stats.optimization_passes);
    printf("Algebraic simplifications: %d\n", opt_stats.algebraic_simp);
    printf("Constant folding operations: %d\n", opt_stats.const_fold);
    printf("Constant propagations: %d\n", opt_stats.const_prop);
    printf("Copy propagations: %d\n", opt_stats.copy_prop);
    printf("Common subexpressions eliminated: %d\n", opt_stats.cse);
    printf("Dead code eliminations: %d\n", opt_stats.dead_code);
    printf("Total optimizations: %d\n", 
           opt_stats.algebraic_simp + opt_stats.const_fold + 
           opt_stats.const_prop + opt_stats.copy_prop + 
           opt_stats.cse + opt_stats.dead_code);
}