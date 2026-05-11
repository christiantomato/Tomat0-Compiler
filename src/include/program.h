/**
 * @file program.h
 * @brief Defines the overall Tomat0 program representation. 
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#include "ast_node.h"

/**
 * @struct Program
 * @brief Completely encapsulates the Tomat0 program. This struct is passed into code generation. 
 */

typedef struct {
    ASTNode* root; /**< The root node of the ast. */
    List* scopes; /**< Complete list of every active scope (so we can free after code generation). */
} Program;

/**
 * @brief Creates the program representation which we will use for code generation.
 * 
 * Root and scopes are passed from the lexer. 
 * 
 * @param root Pointer to the root ast node.
 * @param scopes Pointer to the list of scopes. 
 * @return Pointer to the program struct. 
 */

Program* init_program(ASTNode* root, List* scopes);

/**
 * @brief Frees allocated memory for the program representation. 
 * 
 * Frees entire abstract syntax tree.
 * Frees every scope, its symbol table, and its symbols. 
 * 
 * @param program Pointer to the program. 
 */

void free_program(Program* program);

#endif
