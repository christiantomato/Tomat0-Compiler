/*
AST Node Structure

defines the different types of nodes that the abstract syntax tree will use
these include:
- variable declarations
- print statements
- binary operations
- function calls

NodeType type: the type of ast node it is
List* children: children nodes (used for the program node statements, function node statements, and blocks of code)
ASTSpecialization: a union containing structs for specialized ast nodes
*/

#include "array_list.h"
#include <stdio.h>

#ifndef AST_NODE_H
#define AST_NODE_H

typedef enum ast_node_type {
    AST_PROGRAM,
    AST_VARIABLE_DECLARATION,
    AST_VARIABLE_ASSIGNMENT,
    AST_PRINT_STATEMENT,
    AST_BINARY_OPERATION,
    AST_NEGATION,
    AST_VARIABLE,
    AST_INTEGER,
    AST_STRING
} NodeType;

//forward declare our ast node structure
struct ast_node_struct;

/*
define structs for all specialized ast nodes
*/

typedef struct ast_variable_declaration {
    const char* data_type;
    char* variable_name;
    struct ast_node_struct* assignment;
} VariableDeclaration;

typedef struct ast_variable_assignment {
    char* variable_name;
    struct ast_node_struct* assignment;
} VariableAssignment;

typedef struct ast_print_statement {
    struct ast_node_struct* statement;
} PrintStatement;

typedef struct ast_binary_operation {
    struct ast_node_struct* left;
    struct ast_node_struct* right;
    char* operand;
} BinaryOperation;

typedef struct ast_negation {
    struct ast_node_struct* factor;
} Negation;

typedef struct ast_variable {
    char* variable_name;
    struct ast_node_struct* value;
} Variable;

typedef struct ast_integer {
    int value;
} IntegerLiteral;

typedef struct ast_string {
    char* value;
    unsigned int string_id;
} StringLiteral;

//unionize all the different node types
typedef union ast_node_specializations {
    VariableDeclaration variable_declaration;
    VariableAssignment variable_assignment;
    PrintStatement print_statement;
    BinaryOperation binary_operation;
    Negation negation;
    Variable variable;
    IntegerLiteral integer_literal;
    StringLiteral string_literal;
} ASTSpecialization;

//main ast node structure
typedef struct ast_node_struct {
    NodeType type;
    List* children;
    ASTSpecialization specialization;
} ASTNode;

//initialize a node
ASTNode* init_node(NodeType type);
//get the node type enum name
char* node_type_str(ASTNode* node);
//write to file the tree representation from any node
void print_ast(FILE* file, ASTNode* root, int indent);
//helper method for indentation
void print_indent(FILE* file, int indent);
//free memory of a node
int free_node(ASTNode* node);
//wrapper for free node
void free_node_wrapper(void* node);

#endif
