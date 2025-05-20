/*
 * codegen.h - Assembly code generation module
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "quad.h"
#include "symtab.h"
#include <stdio.h>

// Target architecture options
typedef enum {
    ARCH_X86,       // x86 32-bit
    ARCH_X86_64,    // x86 64-bit
    ARCH_ARM        // ARM
} TargetArchitecture;

// Output format options
typedef enum {
    FORMAT_GAS,      // GNU Assembler
    FORMAT_NASM,     // NASM
    FORMAT_MASM      // Microsoft MASM
} OutputFormat;

// Code generator configuration
typedef struct {
    TargetArchitecture arch;
    OutputFormat format;
    const char* outputFilename;
    int optimizationLevel;
    int debugInfo;
} CodeGenConfig;

// Initialize the code generator with configuration
void initCodeGen(CodeGenConfig* config);

// Generate assembly code from optimized quadruples
void generateAssemblyCode(QuadList* quadList, SymbolTable* symtab);

// Generate assembly code and write to a file
int generateAssemblyFile(QuadList* quadList, SymbolTable* symtab, const char* filename);

// Free resources used by the code generator
void freeCodeGen();

#endif /* CODEGEN_H */ 