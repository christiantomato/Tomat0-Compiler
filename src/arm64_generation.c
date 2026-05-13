/**
 * @file arm64_generation.c
 * @brief ast is traversed and assembly code is generated.
 * 
 * The ARM64 Stack is FULL DESCENDING by convention.
 */

#include "include/arm64_generation.h"
#include "include/codegen_context.h"

/**
 * @brief Allocates space for the local variable on the stack frame.
 * 
 * @param variable Pointer to the variable symbol. 
 */

static void allocate_local_variable(Symbol* variable) {

}

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
 * @brief Collapses the stack frame for the activation record
 * 
 * @param subroutine_scope Pointer to the subroutines scope.
 * @param context Pointer to the code gen context.
 */

static void collapse_stack_frame(Scope* subroutine_scope, CodeGenContext* context) {
    //move stack pointer back to frame pointer and restore fp and lr
    fprintf(context->output, "\tmov sp, fp\n");
    fprintf(context->output, "\tldp fp, lr, [sp], #16\n");
    //return the address
    fprintf(context->output, "\tret\n");
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
            //generate assembly for children statements
            for(int i = 0; i < node->specialization.block.statements->num_items; i++) {
                node_to_asm(node->specialization.block.statements->array[i], context);
            }
            break;
        }
        case AST_ENTRY_POINT: {
            setup_entry_point(context);
            setup_stack_frame(node->scope, context);
            //generate for children statements
            for(int i = 0; i < node->specialization.block.statements->num_items; i++) {
                node_to_asm(node->specialization.block.statements->array[i], context);
            }
            collapse_stack_frame(node->scope, context);
            break;
        }
        case AST_VARIABLE_DECLARATION: {
            //get the variable
            Symbol* variable = lookup_symbol_in_scope(node->scope, node->specialization.var_dec.variable_name);
            //generate the assembly for the assignment
            node_to_asm(node->specialization.var_dec.assignment, context);
            //get whatever the result was
            
            break;
        }
        case AST_PRINT_STATEMENT: {

            break;
        }
        case AST_NUMBER: {

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
