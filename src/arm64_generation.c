/**
 * @file arm64_generation.c
 * @brief ast is traversed and assembly code is generated.
 * 
 * The ARM64 Stack is FULL DESCENDING by convention.
 */

#include "include/arm64_generation.h"
#include "include/codegen_context.h"

/**
 * @brief Sets up the .data section with format strings for printing.
 * 
 * @param context Pointer to the code gen context.
 */

static void setup(CodeGenContext* context) {
    //emit .data section upfront
    fprintf(context->output, ".data\n");
    fprintf(context->output, "fmt_int: .asciz \"%%d\\n\"\n");
    fprintf(context->output, "fmt_str: .asciz \"%%s\\n\"\n");
    fprintf(context->output, "\n");
}

/**
 * @brief Prints out an integer.
 * 
 * @param context Pointer to the code gen context.
 */

static void print_int(CodeGenContext* context) {
    fprintf(context->output, "\t//print integer.\n");
    //load format string address into x0 (printf's address arg) using this weird page off thing since its too far away from text section
    fprintf(context->output, "\tadrp x0, fmt_int@PAGE\n");
    //add some offset thing
    fprintf(context->output, "\tadd x0, x0, fmt_int@PAGEOFF\n");
    //push the int arg to the stack (keep 16 byte aligned)
    fprintf(context->output, "\tstr x%d, [sp, #-16]!\n", context->result_reg);
    //call printf
    fprintf(context->output, "\tbl _printf\n");
    //restore sp
    fprintf(context->output, "\tadd sp, sp, #16\n\n");
    //free the result register
    free_register(context->register_manager, context->result_reg);
}

/**
 * @brief Negates a value.
 * 
 * @param value_reg Register with value to negate.
 * @param context Pointer to the code gen context.
 */

static void negate_value(int value_reg, CodeGenContext* context) {
    fprintf(context->output, "\t//negate value.\n");
    //allocate for result
    context->result_reg = allocate_general_register(context->register_manager);
    fprintf(context->output, "\tneg x%d, x%d\n\n", context->result_reg, value_reg);
}

/**
 * @brief Computes a binary operation.
 * 
 * @param left_reg The left register number.
 * @param right_reg The right register number.
 * @param operator The operator.
 * @param context Pointer to the code gen context.
 */

static void generate_binary_op(int left_reg, int right_reg, char operator, CodeGenContext* context) {
    fprintf(context->output, "\t//binary operation.\n");
    //allocate a register for result
    context->result_reg = allocate_general_register(context->register_manager);

    //switch on operator
    switch(operator) {
        case '+':
            fprintf(context->output, "\tadd x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;
        case '-':
            fprintf(context->output, "\tsub x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;
        case '*':
            fprintf(context->output, "\tmul x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;
        case '/':
            fprintf(context->output, "\tdiv x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;
    }

    //free left and right registers
    free_register(context->register_manager, left_reg);
    free_register(context->register_manager, right_reg);
}


/**
 * @brief Loads a variable (global or local).
 * 
 * @param variable Pointer to the variable.
 * @param context Pointer to the code gen context. 
 */

static void load_variable(Symbol* variable, CodeGenContext* context) {
     //determine the storage type
    if(variable->data.var_sym.storage == STORAGE_GLOBAL) {
        //load from label or whatever
    }
    else if(variable->data.var_sym.storage == STORAGE_LOCAL) {
        //allocate a register to put the variable value
        context->result_reg = allocate_general_register(context->register_manager);
        //load the value into the register from its stack frame
        fprintf(context->output, "\t//load variable from stack.\n");
        fprintf(context->output, "\tldr x%d, [fp, #%d]\n\n", context->result_reg, variable->data.var_sym.offset);
    }
}

/**
 * @brief Stores a variable (global or local).
 * 
 * @param variable Pointer to the variable.
 * @param context Pointer to the code gen context.
 */

static void store_variable(Symbol* variable, CodeGenContext* context) {
    //determine the storage type
    if(variable->data.var_sym.storage == STORAGE_GLOBAL) {
        //create label or whatever
    }
    else if(variable->data.var_sym.storage == STORAGE_LOCAL) {
        //get the offset
        int offset = variable->data.var_sym.offset;
        //store to its stack frame
        fprintf(context->output, "\t//store variable to stack.\n");
        fprintf(context->output, "\tstr x%d, [fp, #%d]\n\n", context->result_reg, offset);
        //free used register
        free_register(context->register_manager, context->result_reg);
    }
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
    fprintf(context->output, "\t//setup stack frame.\n");
    //store frame pointer and link register to stack
    fprintf(context->output, "\tstp fp, lr, [sp, #-16]!\n");
    fprintf(context->output, "\tmov fp, sp\n");

    //allocate space needed for locals, negate since its currently negative
    int locals_space = -subroutine_scope->current_offset;
    //round to 16 byte multiples since stack likes being 16 byte aligned.
    if (locals_space % 16 != 0) {
        locals_space += 16 - (locals_space % 16);
    }
    fprintf(context->output, "\tsub sp, sp, #%d\n\n", locals_space);
}

/**
 * @brief Collapses the stack frame for the activation record
 * 
 * @param subroutine_scope Pointer to the subroutines scope.
 * @param context Pointer to the code gen context.
 */

static void collapse_stack_frame(Scope* subroutine_scope, CodeGenContext* context) {
    fprintf(context->output, "\t//collapse the stack frame.\n");
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
            //setup the .data section first for print formats
            setup(context);
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
        case AST_BLOCK: {
            //TODO: code gen for this case



            break;
        }
        case AST_FUNCTION_DECLARATION: {
            //TODO: code gen for this case


            break;
        }
        case AST_FUNCTION_CALL: {
            //TODO: code gen for this case

            break;
        }
        case AST_VARIABLE_DECLARATION: {
            //get the variable
            Symbol* variable = lookup_symbol_in_scope(node->scope, node->specialization.var_dec.variable_name);
            //generate the assembly for the assignment 
            node_to_asm(node->specialization.var_dec.assignment, context);
            //store result
            store_variable(variable, context);
            break;
        }
        case AST_VARIABLE_ASSIGNMENT: {
            //get the variable
            Symbol* variable = lookup_symbol(node->scope, node->specialization.var_assign.variable_name);
            //evaluate assignment result
            node_to_asm(node->specialization.var_assign.assignment, context);
            //store result
            store_variable(variable, context);
            break;
        }
        case AST_PRINT_STATEMENT: {
            //evaluate the statement
            node_to_asm(node->specialization.print_statement.operand, context);
            print_int(context);
            break;
        }
        case AST_RETURN_STATEMENT: {
            //TODO: code gen for this case

            break;
        }
        case AST_BINARY_OPERATION: {
            //get result registers for left and right
            node_to_asm(node->specialization.binary_op.left, context);
            int left_reg = context->result_reg;
            node_to_asm(node->specialization.binary_op.right, context);
            int right_reg = context->result_reg;
            //generate assembly
            generate_binary_op(left_reg, right_reg, *node->specialization.binary_op.operator, context);
            break;
        }
        case AST_NEGATION: {
            //get result for operand
            node_to_asm(node->specialization.negation.operand, context);
            int value_reg = context->result_reg;
            //generate negate assembly
            negate_value(value_reg, context); 
            break;
        }
        case AST_PARAMETER: {
            //TODO: code gen for this case

            break;
        }
        case AST_VARIABLE: {
            //get the variable
            Symbol* variable = lookup_symbol(node->scope, node->specialization.var.variable_name);
            //load it
            load_variable(variable, context);
            break;
        }
        case AST_NUMBER: {
            //just move the number to the register
            context->result_reg = allocate_general_register(context->register_manager);
            fprintf(context->output, "\t//move number to register.\n");
            fprintf(context->output, "\tmov x%d, #%d\n\n", context->result_reg, node->specialization.num.value);
            break;
        }
        case AST_STRING: {
            //TODO: code gen for this case

            break;
        }
        case AST_THROW: {
            //TODO: code gen for this case

            break;
        }
        case AST_RUNTIME_END: {
            //put 0 in return register x0
            fprintf(context->output, "\t//put status code 0 in ret register.\n");
            fprintf(context->output, "\tmov x0, #0\n");
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
