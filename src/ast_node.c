#include "include/ast_node.h"
#include <stdlib.h>

/*
Initialize AST Node Function

creates an ast node based on the given type

NodeType type: the type of node it will be

return: a pointer to the node
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
            node->specialization.variable.value = NULL;
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
AST Node Type As String Function

converts the node type enum into a string

ASTNode* node: the target node for which we are finding the type

return: type as a string pointer
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

/*
Print Indentation Function

helper method for the print ast function
prints a tab space based on the indentation level (more tabs as we recurse deeper)

FILE* file: the file we are writing the tab spaces to
int indent: the amount of indents
*/

static void print_indent(FILE* file, int indent) {
    for(int i = 0; i < indent; i++) {
        fprintf(file, "\t");
    }
}

/*
Print Abstract Syntax Tree Function

recursively writes the tree representation from the given root to a file in a preorder fashion
levels in the tree are shown using an indentation

FILE* file: the file we are wrting to
ASTNode* root: the root we are starting at (usually starting from the program node)
int indent: variable to keep track of the indentation spaces when we call recursively
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
            //values not determined through parsing actually so this field is unecessary
            fprintf(file, "value = (determined at runtime)\n");
            //below statment crashses as we do not know the value yet
            //print_ast(file, root->specialization.variable.value, indent + 1);
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
Free Node Function

frees dynamically allocated memory for node and children nodes recursively
any token values are freed by the parser, by freeing the entire tokens list
frees entire tree if program node is inputted

ASTNode* node: the root node

return: 0 for success, 1 otherwise
*/

int free_node(ASTNode* node) {
    //make sure not garbage
    if(node == NULL) {
        return 1;
    }
    //free any dynamically allocated memory needed for specialized nodes
    switch(node->type) {
        case AST_VARIABLE_DECLARATION:
            free_node(node->specialization.variable_declaration.assignment);
            break;
        case AST_VARIABLE_ASSIGNMENT:
            free_node(node->specialization.variable_assignment.assignment);
            break;
        case AST_PRINT_STATEMENT:
            free_node(node->specialization.print_statement.statement);
            break;
        case AST_BINARY_OPERATION:
            free_node(node->specialization.binary_operation.left);
            free_node(node->specialization.binary_operation.right);
            break;
        case AST_NEGATION:
            free_node(node->specialization.negation.factor);
            break;
        case AST_VARIABLE:
            free_node(node->specialization.variable.value);
            break;
        case AST_INTEGER: 
            //nothing to free
            break;
        case AST_STRING:
            //nothing to free 
            break;
        default:
            break;
    }
    //free all children nodes recursively
    if(node->children != NULL) {
        free_complex_list(node->children, free_node_wrapper);
    }
    //free node itself
    free(node);
    return 0;
}

/*
Free Node Wrapper Function 

a wrapper function for the free node function, utilized with my built-in array list
to ensure i can free a list of nodes properly

void* node: the node to be freed
*/

void free_node_wrapper(void* node) {
    free_node((ASTNode*) node);
}
