/**
 * @file arm64_generation.c
 * @brief ast is traversed and assembly code is generated.
 * 
 * The ARM64 Stack is FULL DESCENDING by convention.
 */

#include "include/arm64_generation.h"
#include "include/codegen_context.h"

/**
 * @brief Sets up our entry point. 
 * 
 * @param context Pointer to the code gen context.
 */

static void setup_entry_point(CodeGenContext* context) {
    //check if we have created the .text section yet
    if(!context->generate_text) {
        fprintf(context->output, ".text\n");
        context->generate_text = true;
    }

    //specify the entry point
    fprintf(context->output, ".global _main\n\n");
    //add the main label
    fprintf(context->output, "_main:\n");
}

/**
 * @brief Sets up a stack frame for the activation record.
 * 
 * For function calls and the entry point. 
 * 
 * @param subroutine_scope Pointer to the subroutines scope.
 * @param context Pointer to the code gen context.
 */

static void setup_stack_frame(Scope* subroutine_scope, CodeGenContext* context) {
    //store frame pointer and link register to stack
    fprintf(context->output, "\tstp fp, lr, [sp, #-16]!\n");
    fprintf(context->output, "\tmov fp, sp\n");

    //allocate space needed for locals
    int locals_space = subroutine_scope->current_offset;
    fprintf(context->output, "\tsub sp, sp, #%d\n", locals_space);
}

/**
 * @brief Turns in ast node to assembly code.
 * 
 * @param node Pointer to the node.
 * @param context Pointer to the code gen context. 
 */

static void node_to_asm(ASTNode* node, CodeGenContext* context) {
    //prevent null nodes
    if(!node) return; 

    //generate assembly based on the node type
    switch(node->type) {
        case AST_GLOBAL: {
            //go through children
            for(int i = 0; i < node->specialization.block.statements->num_items; i++) {
                node_to_asm(node->specialization.block.statements->array[i], context);
            }
            break;
        }

        case AST_ENTRY_POINT: {
            setup_entry_point(context);
            setup_stack_frame(node->scope, context);
            break;
        }
    }
}

/*
 * Writes the assembly to a file.
 */

void generate_assembly(FILE* file, Program* program) {
    //begin by extracting the root of the tree
    ASTNode* root = program->root;
    //create the code generation context struct
    CodeGenContext* context = init_context(file);
    
    //walk the tree and generate.
    node_to_asm(root, context);

    //free the context struct
    free_context(context);
}
