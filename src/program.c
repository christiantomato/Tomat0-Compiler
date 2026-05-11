/**
 * @file program.c
 * @brief Implements the init and free for the program definition. 
 */

#include "include/program.h"
#include <stdlib.h>

/*
 * Creates and initalizes the program struct. 
 */

Program* init_program(ASTNode* root, List* scopes) {
    //allocate space for the program struct
    Program* program = malloc(sizeof(Program));
    //set root and scopes
    program->root = root;
    program->scopes = scopes;
    return program;
}

/*
 * Frees node and scopes list (done after code gen). 
 */

void free_program(Program* program) {
    //free the ast
    free_node(program->root);
    //free the scopes
    free_list(program->scopes, free_scope_wrapper);
}
