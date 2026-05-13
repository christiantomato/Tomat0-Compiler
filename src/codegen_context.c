/**
 * @file codegen_context.c
 * @brief Implements functions defined in context definition. 
 */

#include "include/codegen_context.h"
#include <stdlib.h>

/*
 * Initalize the context. 
 */

CodeGenContext* init_context(FILE* output_file) {
    //allocate space for the struct
    CodeGenContext* context = malloc(sizeof(CodeGenContext));

    //set members
    context->output = output_file;
    context->register_manager = init_register_manager();
    context->generate_text = false;

    return context;
}

/*
 * Free function. 
 */

void free_context(CodeGenContext* context) {

}
