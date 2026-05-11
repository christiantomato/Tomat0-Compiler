/**
 * @file ast_node.c
 * @brief Implements defined functions for my ast node definition. 
 */

#include "include/ast_node.h"
#include <stdlib.h>

/*
 * Initalizes an ast node based on the given type.
 */

ASTNode* init_node(NodeType type, Scope* scope) {
    //allocate for node using calloc to set default values
    ASTNode* node = calloc(1, sizeof(ASTNode));

    //set type and scope
    node->type = type;
    node->scope = scope;

    return node;
}

/*
 * Converts the node type enum into a string
 */

char* node_type_as_str(ASTNode* node) {
    switch(node->type) {
        case AST_GLOBAL: return "AST_GLOBAL"; break;
        case AST_ENTRY_POINT: return "AST_ENTRY_POINT"; break;
        case AST_BLOCK: return "AST_BLOCK"; break;
        case AST_FUNCTION_DECLARATION: return "AST_FUNCTION_DECLARATION"; break;
        case AST_FUNCTION_CALL: return "AST_FUNCTION_CALL"; break;
        case AST_VARIABLE_DECLARATION: return "AST_VARIABLE_DECLARATION"; break;
        case AST_VARIABLE_ASSIGNMENT: return "AST_VARIABLE_ASSIGNMENT"; break;
        case AST_BINARY_OPERATION: return "AST_BINARY_OPERATION"; break;
        case AST_PRINT_STATEMENT: return "AST_PRINT_STATEMENT"; break;
        case AST_RETURN_STATEMENT: return "AST_RETURN_STATEMENT"; break;
        case AST_NEGATION: return "AST_NEGATION"; break;
        case AST_PARAMETER: return "AST_PARAMETER"; break;
        case AST_VARIABLE: return "AST_VARIABLE"; break;
        case AST_NUMBER: return "AST_NUMBER"; break;
        case AST_STRING: return "AST_STRING"; break;
        case AST_THROW: return "AST_THROW"; break;
        case AST_RUNTIME_END: return "AST_RUNTIME_END";
    }
}

/**
 * @brief Helper method to print tab spaces based on tree depth. 
 * 
 * @param file Pointer to the file we are printing the spaces to. 
 * @param indent Tree level. 
 */

static void print_indent(FILE* file, int indent) {
    for(int i = 0; i < indent; i++) {
        fprintf(file, "\t");
    }
}

/*
 * Recursively writes the tree representation from the given root to a file in a pre-order fashion.
 * Levels in the tree are shown using an indentation.
 */

void print_ast(FILE* file, ASTNode* root, int indent) {
    //always start with printing the indentation and node title
    print_indent(file, indent);
    fprintf(file, "NODE: %s\n", node_type_as_str(root));

    //print additional specfic details based on the node type
    switch(root->type) {
        case AST_GLOBAL:
            //iterate through the statements in the global scope and print
            for(int i = 0; i < root->specialization.block.statements->num_items; i++) {
                //indent always 1 for program children
                print_ast(file, root->specialization.block.statements->array[i], 1);
            }
            break;
        case AST_ENTRY_POINT:
            //go through statements in the main function and print
            for(int i = 0; i < root->specialization.block.statements->num_items; i++) {
                //indent always 1 for program children
                print_ast(file, root->specialization.block.statements->array[i], indent + 1);
            }
            break;
        case AST_BLOCK:
            //print out statements in this block
            for(int i = 0; i < root->specialization.block.statements->num_items; i++) {
                //indent always 1 for program children
                print_ast(file, root->specialization.block.statements->array[i], indent + 1);
            }
            break;
        case AST_FUNCTION_DECLARATION:
            //list out the relevant information, remembering to print current indent
            print_indent(file, indent);
            fprintf(file, "function_name = %s\n", root->specialization.func_dec.function_name);
            print_indent(file, indent);
            fprintf(file, "parameters = \n"); 
            //iterate through parameters
            for(int i = 0; i < root->specialization.func_dec.parameters->num_items; i++) {
                print_ast(file, root->specialization.func_dec.parameters->array[i], indent + 1);
            }
            print_indent(file, indent);
            fprintf(file, "return_type = %s\n", data_type_as_str(root->specialization.func_dec.return_type));
            print_indent(file, indent);
            fprintf(file, "code_block: \n");
            //print out whole code block
            print_ast(file, root->specialization.func_dec.code_block, indent + 1);
        case AST_FUNCTION_CALL:
            print_indent(file, indent);
            fprintf(file, "function_name = %s\n", root->specialization.func_call.function_name);
            print_indent(file, indent);
            fprintf(file, "parameter_values = \n");
            //iterate through parameter values
            for(int i = 0; i < root->specialization.func_call.parameter_inputs->num_items; i++) {
                print_ast(file, root->specialization.func_call.parameter_inputs->array[i], indent + 1);
            }
        case AST_VARIABLE_DECLARATION:
            print_indent(file, indent);
            fprintf(file, "data_type = %s\n", data_type_as_str(root->specialization.var_dec.data_type));
            print_indent(file, indent);
            fprintf(file, "variable_name = %s\n", root->specialization.var_dec.variable_name);
            //assignment is also a node, so recurse into that
            print_indent(file, indent);
            fprintf(file, "assignment = \n");
            print_ast(file, root->specialization.var_dec.assignment, indent + 1);
            break;
        case AST_VARIABLE_ASSIGNMENT:
            print_indent(file, indent);
            fprintf(file, "name = %s\n", root->specialization.var_assign.variable_name);
            print_indent(file, indent);
            fprintf(file, "assignment = \n");
            print_ast(file, root->specialization.var_assign.assignment, indent + 1);
            break;
        case AST_BINARY_OPERATION:
            print_indent(file, indent);
            fprintf(file, "left = \n");
            print_ast(file, root->specialization.binary_op.left, indent + 1);
            print_indent(file, indent);
            fprintf(file, "right = \n");
            print_ast(file, root->specialization.binary_op.right, indent + 1);
            print_indent(file, indent);
            fprintf(file, "operator = %s\n", root->specialization.binary_op.operator);
            break;
        case AST_PRINT_STATEMENT:
            print_indent(file, indent);
            fprintf(file, "output = \n");
            print_ast(file, root->specialization.print_statement.operand, indent + 1);
            break;
        case AST_RETURN_STATEMENT:
            print_indent(file, indent);
            fprintf(file, "return = \n");
            print_ast(file, root->specialization.print_statement.operand, indent + 1);
            break;
        case AST_NEGATION:
            print_indent(file, indent);
            fprintf(file, "negate = \n");
            print_ast(file, root->specialization.negation.operand, indent + 1);
            break;
        case AST_PARAMETER:
            print_indent(file, indent);
            fprintf(file, "parameter_type = %s\n", data_type_as_str(root->specialization.param.parameter_type));
            print_indent(file, indent);
            fprintf("parameter_name = %s\n", root->specialization.param.parameter_name);
            break;
        case AST_VARIABLE:
            print_indent(file, indent);
            fprintf(file, "variable_name = %s\n", root->specialization.var.variable_name);
            break;
        case AST_NUMBER:
            print_indent(file, indent);
            fprintf(file, "value = %d\n", root->specialization.num.value);
            break;
        case AST_STRING:
            print_indent(file, indent);
            fprintf(file, "value = '%s'\n", root->specialization.string.value);
            break;
        default:
            //will just print out ast type name.
            break;
    }
}

/*
 * Frees dynamically allocated memory for nodes recursively, and for children iteratively. 
 * Allocated memory for members of specialized ast nodes (like var names) are also freed. 
 * Frees entire tree if program node is inputted. 
 */

void free_node(ASTNode* node) {
    //for non terminal nodes, free members which have allocated memory. 
    switch(node->type) {
        case AST_GLOBAL: 
            //free all the statements inside the global scope
            free_list(node->specialization.block.statements, free_node_wrapper);
        case AST_ENTRY_POINT:
            //free all main entry instructions
            free_list(node->specialization.block.statements, free_node_wrapper);
        case AST_BLOCK:
            //free all statements
            free_list(node->specialization.block.statements, free_node_wrapper);
        case AST_FUNCTION_DECLARATION:
            //free name, ast params, and code block
            free(node->specialization.func_dec.function_name);
            free_list(node->specialization.func_dec.parameters, free_node_wrapper);
            free_node(node->specialization.func_dec.code_block);
        case AST_FUNCTION_CALL:
            //free name and param inputs
            free(node->specialization.func_call.function_name);
            free_list(node->specialization.func_call.parameter_inputs, free_node_wrapper);
        case AST_VARIABLE_DECLARATION:
            //free name and assignment
            free(node->specialization.var_dec.variable_name);
            free_node(node->specialization.var_dec.assignment);
            break;
        case AST_VARIABLE_ASSIGNMENT:
            //free name and assignment
            free(node->specialization.var_assign.variable_name);
            free_node(node->specialization.var_assign.assignment);
            break;
        case AST_BINARY_OPERATION:
            //free operator and nodes
            free(node->specialization.binary_op.operator);
            free_node(node->specialization.binary_op.left);
            free_node(node->specialization.binary_op.right);
            break;
        case AST_PRINT_STATEMENT:
            //free operand
            free_node(node->specialization.print_statement.operand);
            break;
        case AST_RETURN_STATEMENT: 
            free_node(node->specialization.return_statement.operand);
        case AST_NEGATION:
            free_node(node->specialization.negation.operand);
            break;
        case AST_PARAMETER:
            //free name
            free(node->specialization.param.parameter_name);
        case AST_VARIABLE:
            //free name
            free(node->specialization.var.variable_name);
            break;
        case AST_NUMBER: 
            //nothing to free
            break;
        case AST_STRING:
            free(node->specialization.string.value);
            break;
        default:
            break;
    }

    //free node itself
    free(node);
}

/*
 * A wrapper function for the free node function, utilized with my built-in array list
 * to ensure I can free a list of nodes properly. 
 */

void free_node_wrapper(void* node) {
    free_node((ASTNode*) node);
}
