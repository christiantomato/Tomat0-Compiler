/**
 * @file ast_node.h
 * @brief Defines the different types of nodes that the abstract syntax tree will use. 
 * 
 * These include: 
 * - variable declarations
 * - print statements
 * - binary operations
 * - binary operations
 * - function calls
 */

#include "array_list.h"
#include <stdio.h>

#ifndef AST_NODE_H
#define AST_NODE_H

/**
 * @enum NodeType
 * @brief Represents all possible node types that can be parsed. 
 */

typedef enum {
    AST_GLOBAL,
    AST_VARIABLE_DECLARATION,
    AST_VARIABLE_ASSIGNMENT,
    AST_PRINT_STATEMENT,
    AST_BINARY_OPERATION,
    AST_NEGATION,
    AST_VARIABLE,
    AST_INTEGER,
    AST_STRING
} NodeType;

//Forward declare the general ast node structure, so we can use them when defining specific ast node structs.  
struct ast_node_struct;

//NON TERMINAL NODES: 

/**
 * @struct VariableDeclaration
 * @brief Represents a variable declaration instruction. 
 * 
 * Based on this definition, variables must be assigned an inital value when declared. 
 */

typedef struct {
    const char* data_type; /**< String representation of the data type. */
    char* variable_name; /**< The name of the variable */
    struct ast_node_struct* assignment; /**< The assignment which may be a terminal or non-terminal node. */
} VariableDeclaration;

/**
 * @struct VariableAssignment
 * @brief Represents a variable assignment instruction. 
 */

typedef struct {
    char* variable_name; /**< The variable being assigned a value. */
    struct ast_node_struct* assignment; /**< The assignement node. */
} VariableAssignment;

/**
 * @struct PrintStatement
 * @brief Represents a print statement instruction. 
 */

typedef struct {
    struct ast_node_struct* statement; /**< The statement to print. */
} PrintStatement;

/**
 * @struct BinaryOperation
 * @brief Represents a binary operation instruction between 2 nodes. 
 * 
 * Current supported operators include: 
 * - Addition: +
 * - Subtraction: -
 * - Multiplication: *
 * - Division: / 
 */

typedef struct {
    struct ast_node_struct* left; /**< The left operand. */
    struct ast_node_struct* right; /**< The right operand.  */
    char* operator; /**< The operator. */
} BinaryOperation;

/**
 * @struct Negation
 * @brief Represents a negation instruction. 
 */

typedef struct {
    struct ast_node_struct* factor; /**< The factor being negated. */
} Negation;

//TERMINAL NODES: 

/**
 * @struct Variable
 * @brief Encodes a variable
 */

typedef struct {
    char* variable_name; /**< Variable name. */
} Variable;

/**
 * @struct IntegerLiteral
 * @brief Represents an integer.  
 */

typedef struct {
    int value; /**< The integer value. */
} IntegerLiteral;

/**
 * @struct StringLiteral
 * @brief Represents a string literal. 
 */

typedef struct {
    char* value; /**< The string. */
    unsigned int string_id; /**< The id for the string. */
} StringLiteral;

/**
 * @union ASTSpecialization
 * @brief A union so a node can choose its specific type. 
 */

typedef union {
    VariableDeclaration variable_declaration;
    VariableAssignment variable_assignment;
    PrintStatement print_statement;
    BinaryOperation binary_operation;
    Negation negation;
    Variable variable;
    IntegerLiteral integer_literal;
    StringLiteral string_literal;
} ASTSpecialization;

/**
 * @brief A general node definition. 
 * 
 * Utilizes the union to be memory efficient. Node only utilizes memory needed for its specialization.
 */

typedef struct ast_node_struct {
    NodeType type; /**< The type of node. */
    List* children; /**< Pointer to the nodes children (if any). */
    ASTSpecialization specialization; /**< Its specialization from the union. */
} ASTNode;

/**
 * @brief Creates and initalizes a new node. 
 * 
 * @param type The node type. 
 * @return Pointer to the newly created node. 
 */

ASTNode* init_node(NodeType type);

/**
 * @brief Returns the enum name for the node as a string. 
 * 
 * @param node Pointer to the node. 
 * @return String corresponding to the enum name. 
 */

char* node_type_as_str(ASTNode* node);

/**
 * @brief Writes the tree representation for the inputted node. 
 * 
 * @param file Pointer to the file. 
 * @param root Node we are starting at. 
 * @param indent Incremented when print_ast is recursively called. 
 */

void print_ast(FILE* file, ASTNode* root, int indent);

/**
 * @brief Frees the allocated memory for a node. 
 * 
 * @param node Pointer to the node. 
 * @return 0 for success, 1 otherwise. 
 */

int free_node(ASTNode* node);

/**
 * @brief Wrapper for the free node function. 
 * 
 * @param node Pointer to the node. 
 */

void free_node_wrapper(void* node);

#endif
