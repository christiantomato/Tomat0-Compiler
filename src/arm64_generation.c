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

    //emit string literals
    for(int i = 0; i < context->string_literals->num_items; i++) {
        fprintf(context->output, "str%d: .asciz \"%s\\n\"\n", i, (char*) context->string_literals->array[i]);
    }
    fprintf(context->output, "\n");
    
    //start the .text section after all data is emitted
    fprintf(context->output, ".text\n");
    //specify the entry point
    fprintf(context->output, ".global _main\n\n");
}

/**
 * @brief Prints out an integer.
 * 
 * @param context Pointer to the code gen context.
 */

static void print_int(CodeGenContext* context) {
    //save result to new register in case its a return value in x0.
    int safe_reg = allocate_general_register(context->register_manager);
    fprintf(context->output, "\t//ensure int to print doesn't get clobbered.\n");
    fprintf(context->output, "\tmov x%d, x%d\n", safe_reg, context->result_reg);
    //free result register
    free_register(context->register_manager, context->result_reg);

    fprintf(context->output, "\t//print integer.\n");
    //load format string address into x0 (printf's address arg) using this weird page off thing since its too far away from text section
    fprintf(context->output, "\tadrp x0, fmt_int@PAGE\n");
    //add some offset thing
    fprintf(context->output, "\tadd x0, x0, fmt_int@PAGEOFF\n");
    //push the int arg to the stack (keep 16 byte aligned)
    fprintf(context->output, "\tstr x%d, [sp, #-16]!\n", safe_reg);
    //call printf
    fprintf(context->output, "\tbl _printf\n");
    //restore sp
    fprintf(context->output, "\tadd sp, sp, #16\n\n");
    //free safe reg
    free_register(context->register_manager, safe_reg);
}

/**
 * @brief Prints out a string.
 * 
 * @param context Pointer to the code gen context.
 */

static void print_string(CodeGenContext* context) {
    fprintf(context->output, "\t//print string.\n");
    //put the string address in x0 and call
    fprintf(context->output, "\tmov x0, x%d\n", context->result_reg);
    fprintf(context->output, "\tbl _printf\n\n");
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
        //ALGEBRA:
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
            fprintf(context->output, "\tsdiv x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;

        //BOOLEAN ALGEBRA:
        //or case
        case 'o':
            fprintf(context->output, "\torr x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;
        //and case
        case 'a':
            fprintf(context->output, "\tand x%d, x%d, x%d\n\n", context->result_reg, left_reg, right_reg);
            break;

        //RELATIONAL:
        //equals case
        case 'e': 
            fprintf(context->output, "\tcmp x%d, x%d\n", left_reg, right_reg);
            fprintf(context->output, "\tcset x%d, eq\n\n", context->result_reg);
            break;
        case '<':
            fprintf(context->output, "\tcmp x%d, x%d\n", left_reg, right_reg);
            fprintf(context->output, "\tcset x%d, lt\n\n", context->result_reg);
            break;
        case '>':
            fprintf(context->output, "\tcmp x%d, x%d\n", left_reg, right_reg);
            fprintf(context->output, "\tcset x%d, gt\n\n", context->result_reg);
            break;

        default: return;
    }

    //free left and right registers
    free_register(context->register_manager, left_reg);
    free_register(context->register_manager, right_reg);
}


/**
 * @brief Loads a variable (string or regular).
 * 
 * @param variable Pointer to the variable.
 * @param context Pointer to the code gen context. 
 */

static void load_variable(Symbol* variable, CodeGenContext* context) {
    //figure out where to load from
    if(variable->kind == SYMBOL_STRING) {
        //load from label.
        context->result_reg = allocate_general_register(context->register_manager);
        fprintf(context->output, "\t//load string address from label.\n");
        fprintf(context->output, "\tadrp x%d, %s@PAGE\n", context->result_reg, variable->data.str_data.label);
        fprintf(context->output, "\tadd x%d, x%d, %s@PAGEOFF\n\n", context->result_reg, context->result_reg, variable->data.str_data.label);
    }
    else {
        //get the offset
        int offset;
        //check if param or variable
        if(variable->kind == SYMBOL_PARAMETER) offset = variable->data.param_data.offset;
        else if(variable->kind == SYMBOL_VARIABLE) offset = variable->data.var_data.offset;
        else return;

        //allocate a register to put the variable value
        context->result_reg = allocate_general_register(context->register_manager);
        //load the value into the register from its stack frame
        fprintf(context->output, "\t//load variable from stack.\n");
        fprintf(context->output, "\tldr x%d, [fp, #%d]\n\n", context->result_reg, offset);
    }
}

/**
 * @brief Stores a variable.
 * 
 * @param variable Pointer to the variable.
 * @param context Pointer to the code gen context.
 */

static void store_variable(Symbol* variable, CodeGenContext* context) {
    //get the offset
    int offset;
    //check if param or variable
    if(variable->kind == SYMBOL_PARAMETER) offset = variable->data.param_data.offset;
    else if(variable->kind == SYMBOL_VARIABLE) offset = variable->data.var_data.offset;
    //store to its stack frame
    fprintf(context->output, "\t//store variable to stack.\n");
    fprintf(context->output, "\tstr x%d, [fp, #%d]\n\n", context->result_reg, offset);
    //free used register
    free_register(context->register_manager, context->result_reg);
}

/**
 * @brief Spills parameters to stack. 
 * 
 * @param params List of parameters to spill.
 * @param context Pointer to the code gen context. 
 */

static void spill_params(List* params, CodeGenContext* context) {
    for(int i = 0; i < params->num_items; i++) {
        //store param value to stack
        Symbol* param_sym = (Symbol*) params->array[i];
        fprintf(context->output, "\t//spill param to stack.\n");
        fprintf(context->output, "\tstr x%d, [fp, #%d]\n\n", param_sym->data.param_data.reg, param_sym->data.param_data.offset);
    }
}

/**
 * @brief Sets up a stack frame for the activation record.
 * 
 * For function declarations and the entry point. 
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
    fprintf(context->output, "\tret\n\n");
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
            //emit main label
            fprintf(context->output, "_main:\n");
            setup_stack_frame(node->scope, context);
            //generate for children statements
            for(int i = 0; i < node->specialization.block.statements->num_items; i++) {
                node_to_asm(node->specialization.block.statements->array[i], context);
            }
            collapse_stack_frame(node->scope, context);
            break;
        }
        case AST_BLOCK: {
            //loop through statements and generate assembly
            for(int i = 0; i < node->specialization.block.statements->num_items; i++) {
                node_to_asm(node->specialization.block.statements->array[i], context);
            }
            break;
        }
        case AST_FUNCTION_DECLARATION: {
            //emit label
            fprintf(context->output, "_%s:\n", node->specialization.func_dec.function_name);
            //setup stack frame with function scope
            setup_stack_frame(node->specialization.func_dec.code_block->scope, context);
            //get the function symbol
            Symbol* func_symbol = lookup_symbol_in_scope(node->scope, node->specialization.func_dec.function_name);
            //spill parameters to stack
            spill_params(func_symbol->data.func_data.parameters, context);
            //generate code
            node_to_asm(node->specialization.func_dec.code_block, context);
            //check if its void so we can collapse
            if(func_symbol->data.func_data.return_type == TYPE_VOID) collapse_stack_frame(node->scope, context);
            break;
        }
        case AST_FUNCTION_CALL: {
            //evaluate and move each argument into param registers
            for(int i = 0; i < node->specialization.func_call.parameter_inputs->num_items; i++) {
                node_to_asm(node->specialization.func_call.parameter_inputs->array[i], context);
                //allocate a param register
                int param_reg = allocate_param_register(context->register_manager);
                fprintf(context->output, "\t//pass parameter.\n");
                fprintf(context->output, "\tmov x%d, x%d\n\n", param_reg, context->result_reg);
                free_register(context->register_manager, context->result_reg);
            }
            //branch with link
            fprintf(context->output, "\t//call function.\n");
            fprintf(context->output, "\tbl _%s\n\n", node->specialization.func_call.function_name);
            //free all param registers after call (not x0 tho since its being used for return)
            for(int i = 1; i < node->specialization.func_call.parameter_inputs->num_items; i++) {
                free_register(context->register_manager, i);
            }
            //result comes back to x0 (if there is a return)
            context->result_reg = 0;
            break;
        }
        case AST_IF_STATEMENT: {
            //generate code for the condition
            node_to_asm(node->specialization.if_statement.condition, context);

            break;
        }
        case AST_WHILE_LOOP: {



            break;
        }
        case AST_VARIABLE_DECLARATION: {
            //get the variable
            Symbol* variable = lookup_symbol_in_scope(node->scope, node->specialization.var_dec.variable_name);
            //skip strings
            if(variable-> kind == SYMBOL_STRING) break;
            //generate the assembly for the assignment 
            node_to_asm(node->specialization.var_dec.assignment, context);
            //store result
            store_variable(variable, context);
            break;
        }
        case AST_VARIABLE_ASSIGNMENT: {
            //get the variable
            Symbol* variable = lookup_symbol(node->scope, node->specialization.var_assign.variable_name);
            //skip strings (they are immutable)
            if(variable->kind == SYMBOL_STRING) break;
            //evaluate assignment result
            node_to_asm(node->specialization.var_assign.assignment, context);
            //store result
            store_variable(variable, context);
            break;
        }
        case AST_PRINT_STATEMENT: {
            //evaluate the statement
            node_to_asm(node->specialization.print_statement.operand, context);
            //print based on type
            if(node->specialization.print_statement.type == TYPE_INT) print_int(context);
            else if(node->specialization.print_statement.type == TYPE_STRING) print_string(context);
            break;
        }
        case AST_RETURN_STATEMENT: {
            //evaluate the statement
            node_to_asm(node->specialization.return_statement.operand, context);
            //move result to x0
            fprintf(context->output, "\t//return into x0.\n");
            fprintf(context->output, "\tmov x0, x%d\n\n", context->result_reg);
            //free old reg
            free_register(context->register_manager, context->result_reg);
            //collapse the stack frame
            collapse_stack_frame(node->scope, context);
            break;
        }
        case AST_BINARY_OPERATION: {
            //get result registers for left and right
            node_to_asm(node->specialization.binary_op.left, context);
            int left_reg = context->result_reg;
            node_to_asm(node->specialization.binary_op.right, context);
            int right_reg = context->result_reg;
            //generate assembly
            generate_binary_op(left_reg, right_reg, node->specialization.binary_op.operator[0], context);
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
            //nothing to do 
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
            //allocate register for string address
            context->result_reg = allocate_general_register(context->register_manager);
            //figure out what the label is from string id
            char label[8];
            snprintf(label, sizeof(label), "str%d", node->specialization.string.id);
            //load
            fprintf(context->output, "\t//load string address from label.\n");
            fprintf(context->output, "\tadrp x%d, %s@PAGE\n", context->result_reg, label);
            fprintf(context->output, "\tadd x%d, x%d, %s@PAGEOFF\n\n", context->result_reg, context->result_reg, label);
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
    //give context the strings
    context->string_literals = program->strings;
    
    //walk the tree and generate.
    node_to_asm(root, context);

    //free the context struct
    free_context(context);
}
