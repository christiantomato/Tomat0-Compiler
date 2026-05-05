/**
 * @file ast_node.c
 * @brief Implements defined functions for my ast node definition. 
 */

#include "include/ast_node.h"
#include <stdlib.h>

/*
 * Initalizes an ast node based on the given type.
 */

ASTNode* init_node(NodeType type) {
    //allocate memory for the node
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    //start off with a children list of size 3 (most nodes don't actually use the children list anyways)
    node->children = init_list(3);
    //initialize values needed for each node specilization
    switch(type) {
        case AST_VARIABLE_DECLARATION:
            node->specialization.variable_declaration.data_type = NULL;
            node->specialization.variable_declaration.variable_name = NULL;
            node->specialization.variable_declaration.assignment = NULL;
            break;
        case AST_VARIABLE_ASSIGNMENT:
            node->specialization.variable_assignment.variable_name = NULL;
            node->specialization.variable_assignment.assignment = NULL;
            break;
        case AST_PRINT_STATEMENT:
            node->specialization.print_statement.statement = NULL;
            break;
        case AST_BINARY_OPERATION:
            node->specialization.binary_operation.left = NULL;
            node->specialization.binary_operation.right = NULL;
            node->specialization.binary_operation.operator = NULL;
            break;
        case AST_NEGATION:
            node->specialization.negation.factor = NULL;
            break;
        case AST_VARIABLE:
            node->specialization.variable.variable_name = NULL;
            break;
        case AST_INTEGER:
            node->specialization.integer_literal.value = 0;
            break;
        case AST_STRING:
            node->specialization.string_literal.value = "\0";
            node->specialization.string_literal.string_id = 0;
            break;
        default:
            //specialization not needed (ex. AST_PROGRAM)
            break;
    }
    return node;
}

/*
 * Converts the node type enum into a string
 */

char* node_type_as_str(ASTNode* node) {
    switch(node->type) {
        case AST_PROGRAM: return "AST_PROGRAM"; break;
        case AST_VARIABLE_DECLARATION: return "AST_VARIABLE_DECLARATION"; break;
        case AST_VARIABLE_ASSIGNMENT: return "AST_VARIABLE_ASSIGNMENT"; break;
        case AST_PRINT_STATEMENT: return "AST_PRINT_STATEMENT"; break;
        case AST_BINARY_OPERATION: return "AST_BINARY_OPERATION"; break;
        case AST_NEGATION: return "AST_NEGATION"; break;
        case AST_VARIABLE: return "AST_VARIABLE"; break;
        case AST_INTEGER: return "AST_INTEGER"; break;
        case AST_STRING: return "AST_STRING"; break;
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
        case AST_PROGRAM:
            //iterate through the children statements and write them to file
            for(int i = 0; i < root->children->num_items; i++) {
                //indent always 1 for program children
                print_ast(file, root->children->array[i], 1);
            }
            break;   
        case AST_VARIABLE_DECLARATION:
            //list out the relevant information, remembering to indent our information before each print statement
            print_indent(file, indent);
            fprintf(file, "data_type = %s\n", root->specialization.variable_declaration.data_type);
            print_indent(file, indent);
            fprintf(file, "variable_name = %s\n", root->specialization.variable_declaration.variable_name);
            //assignment is also a node, so recurse into that
            print_indent(file, indent);
            fprintf(file, "assignment = \n");
            print_ast(file, root->specialization.variable_declaration.assignment, indent + 1);
            break;
        case AST_VARIABLE_ASSIGNMENT:
            print_indent(file, indent);
            fprintf(file, "name = %s\n", root->specialization.variable_assignment.variable_name);
            print_indent(file, indent);
            fprintf(file, "assignment = \n");
            print_ast(file, root->specialization.variable_assignment.assignment, indent + 1);
            break;
        case AST_PRINT_STATEMENT:
            print_indent(file, indent);
            fprintf(file, "output = \n");
            print_ast(file, root->specialization.print_statement.statement, indent + 1);
            break;
        case AST_BINARY_OPERATION:
            print_indent(file, indent);
            fprintf(file, "left = \n");
            print_ast(file, root->specialization.binary_operation.left, indent + 1);
            print_indent(file, indent);
            fprintf(file, "right = \n");
            print_ast(file, root->specialization.binary_operation.right, indent + 1);
            print_indent(file, indent);
            fprintf(file, "operator = %s\n", root->specialization.binary_operation.operator);
            break;
        case AST_NEGATION:
            print_indent(file, indent);
            fprintf(file, "negate (-) = \n");
            print_ast(file, root->specialization.negation.factor, indent + 1);
            break;
        case AST_VARIABLE:
            print_indent(file, indent);
            fprintf(file, "variable_name = %s\n", root->specialization.variable.variable_name);
            print_indent(file, indent);
            //values not determined through parsing actually so this is unecessary
            //fprintf(file, "value = (determined at runtime)\n");
            break;
        case AST_INTEGER:
            print_indent(file, indent);
            fprintf(file, "value = %d\n", root->specialization.integer_literal.value);
            break;
        case AST_STRING:
            print_indent(file, indent);
            fprintf(file, "value = '%s'\n", root->specialization.string_literal.value);
            print_indent(file, indent);
            fprintf(file, "string_id = %d\n", root->specialization.string_literal.string_id);
            break;
    }
}

/*
 * Frees dynamically allocated memory for nodes recursively, and for children iteratively. 
 * Allocated memory for members of specialized ast nodes (like var names) are also freed. 
 * Frees entire tree if program node is inputted. 
 */

int free_node(ASTNode* node) {
    //make sure not garbage
    if(node == NULL) {
        return 1;
    }
    //for non terminal nodes, free members which have allocated memory. 
    switch(node->type) {
        case AST_VARIABLE_DECLARATION:
            free_node(node->specialization.variable_declaration.assignment);
            free(node->specialization.variable_declaration.variable_name);
            break;
        case AST_VARIABLE_ASSIGNMENT:
            free_node(node->specialization.variable_assignment.assignment);
            free(node->specialization.variable_assignment.variable_name);
            break;
        case AST_PRINT_STATEMENT:
            free_node(node->specialization.print_statement.statement);
            break;
        case AST_BINARY_OPERATION:
            free_node(node->specialization.binary_operation.left);
            free_node(node->specialization.binary_operation.right);
            free(node->specialization.binary_operation.operator);
            break;
        case AST_NEGATION:
            free_node(node->specialization.negation.factor);
            break;
        case AST_VARIABLE:
            free(node->specialization.variable.variable_name);
            break;
        case AST_INTEGER: 
            //nothing to free.
            break;
        case AST_STRING:
            free(node->specialization.string_literal.value);
            break;
        default:
            break;
    }
    //free the list of children nodes (if any) iteratively with free list. 
    if(node->children != NULL) {
        free_list(node->children, free_node_wrapper);
    }
    //free node itself
    free(node);
    return 0;
}

/*
 * A wrapper function for the free node function, utilized with my built-in array list
 * to ensure I can free a list of nodes properly. 
 */

void free_node_wrapper(void* node) {
    free_node((ASTNode*) node);
}
