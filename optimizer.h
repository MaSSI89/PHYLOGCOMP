/*
 * optimizer.h - Header for advanced code optimization module
 */

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "quad.h"  // For Quadruple definition
#include <stdbool.h>

// Optimization flags - can be combined with bitwise OR
typedef enum {
    OPT_NONE               = 0x00,
    OPT_ALGEBRAIC          = 0x01,  // Algebraic simplifications
    OPT_COPY_PROPAGATION   = 0x02,  // Copy propagation
    OPT_CSE                = 0x04,  // Common subexpression elimination
    OPT_CONST_PROPAGATION  = 0x08,  // Constant propagation
    OPT_CONST_FOLDING      = 0x10,  // Constant folding
    OPT_DEAD_CODE          = 0x20,  // Dead code elimination
    OPT_ALL                = 0xFF   // All optimizations
} OptimizationFlags;

// Statistics about optimization results
typedef struct {
    int total_quads;           // Original number of quadruples
    int active_quads;          // Remaining active quadruples
    int eliminated_quads;      // Number of eliminated quadruples
    int algebraic_simp;        // Number of algebraic simplifications
    int copy_prop;             // Number of copy propagations
    int const_prop;            // Number of constant propagations
    int const_fold;            // Number of constant foldings
    int cse;                   // Number of common subexpressions eliminated
    int dead_code;             // Number of dead code eliminations
    int optimization_passes;   // Number of optimization passes performed
} OptimizationStats;

// Optimize the given QuadList with all optimizations
void optimize_code(QuadList* quad_list);

// Optimize with specific flags
void optimize_code_with_flags(QuadList* quad_list, OptimizationFlags flags);

// Get the optimization statistics
OptimizationStats get_optimization_stats();

// Print the optimized code to stdout
void print_optimized_code(QuadList* quad_list);

#endif /* OPTIMIZER_H */ 