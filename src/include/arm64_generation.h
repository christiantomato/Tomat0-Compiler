/**
 * @file arm64_generation.h
 * @brief The code generation phase. 
 */

#ifndef ARM64_GENERATION_H
#define ARM64_GENERATION_H

#include "program.h"

/**
 * @brief Generates the ARM64 assembly by walking the abstract syntax tree.
 * 
 * @param file Pointer to the output .s file.
 * @param program Pointer to the program struct, containing root node.
 */

void generate_assembly(FILE* file, Program* program);

#endif
