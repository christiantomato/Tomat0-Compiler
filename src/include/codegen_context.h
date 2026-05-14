/**
 * @file codegen_context.h
 * @brief Defines a struct for code gen.
 */

#ifndef CODEGEN_CONTEXT_H
#define CODEGEN_CONTEXT_H

#include "register_manager.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * @struct CodeGenContext
 * @brief Encapsulates all data that will be needed to be pass through code gen recursive traversal.
 */

typedef struct {
    FILE* output; /**< The assembly output file. */
    RegisterManager* register_manager; /**< Responsible for managing register use. */
    bool generate_text; /**< Flag to determine when to move from .data to .text. */ 
    int result_reg; /**< Stores the register number of the most recently computed value. */
} CodeGenContext;

/**
 * @brief Initalizes the context struct
 * 
 * @param output_file Pointer to the file where assembly is being written.
 * @return Pointer to the created context. 
 */

CodeGenContext* init_context(FILE* output_file);

/**
 * @brief Frees memory allocated by the context. 
 * 
 * @param context Pointer to the context struct.
 */

void free_context(CodeGenContext* context);

#endif
