/**
 * @file ast_node.h
 * @brief Defines the different types of nodes that the abstract syntax tree will use. 
 * 
 * These include: 
 * - variable declarations
 * - function declarations
 * - print statements
 * - binary operations
 * - and more
 */

#ifndef AST_NODE_H
#define AST_NODE_H

#include "array_list.h"
#include "types.h"
#include "scope.h"

/**
 * @enum NodeType
 * @brief Represents all possible node types in our abstract syntax tree. 
 */

typedef enum {
    AST_GLOBAL,
    AST_ENTRY_POINT,
    AST_BLOCK,
    AST_FUNCTION_DECLARATION,
    AST_FUNCTION_CALL,
    AST_IF_STATEMENT,
    AST_WHILE_LOOP,
    AST_VARIABLE_DECLARATION,
    AST_VARIABLE_ASSIGNMENT,
    AST_BINARY_OPERATION,
    AST_PRINT_STATEMENT,
    AST_RETURN_STATEMENT,
    AST_NEGATION,
    AST_PARAMETER,
    AST_VARIABLE,
    AST_NUMBER,
    AST_STRING,
    AST_THROW,
    AST_RUNTIME_END
} NodeType;

//Forward declare the general ast node structure, so we can use them when defining specific ast node structs.  
struct ast_node_struct;

//NON TERMINAL NODES:

/**
 * @struct Block
 * @brief Represents a code block of instructions.
 * 
 * Structure used by global, entry point, functions, ifs and whiles. 
 */

typedef struct {
    List* statements; /**< A list of ast nodes for all the statements in the block. */
} Block;

/**
 * @struct FunctionDeclaration
 * @brief Represents a function declaration. 
 */

typedef struct {
    char* function_name; /**< The function name. */
    List* parameters; /**< The parameter list (which will contain AST_PARAMETER nodes). */
    DataType return_type; /**< The return type. */
    struct ast_node_struct* code_block; /**< The actual function code. */
} FunctionDeclaration;

/**
 * @struct FunctionCall
 * @brief Represents a function call.
 * 
 * Passed in parameters must be factors.
 */

typedef struct {
    char* function_name; /**< The function that is being called. */
    List* parameter_inputs; /**< List of AST nodes that are passed in for the parameters. */
} FunctionCall;

/**
 * @struct IfStatement
 * @brief Represents an if statement.
 */

typedef struct {
    struct ast_node_struct* condition; /**< The execution condition. */
    struct ast_node_struct* code_block; /**< Block which executes on true. */
    struct ast_node_struct* else_block; /**< Optional block which executes on the complement of condition. Can be another if block! */
} IfStatement;

/**
 * @struct WhileLoop
 * @brief Represents a while loop (similar to if statement)
 */

typedef struct {
    struct ast_node_struct* condition; /**< The execution condition */
    struct ast_node_struct* code_block; /**< Block which loops until condition is false. */
} WhileLoop;

/**
 * @struct VariableDeclaration
 * @brief Represents a variable declaration instruction. 
 * 
 * Based on this definition, variables must be assigned an inital value when declared. 
 */

typedef struct {
    DataType data_type; /**< The type of data. */
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
 * @struct BinaryOperation
 * @brief Represents a binary operation instruction between 2 nodes. 
 * 
 * Current supported operators include:
 * Algebra: 
 * - Addition: +
 * - Subtraction: -
 * - Multiplication: *
 * - Division: / 
 * Boolean Algebra:
 * - Conjunction: and
 * - Disjunction: or
 * Relational Operators:
 * - Equality: equals
 * - Less Than: <
 * - Greater Than: >
 */

typedef struct {
    struct ast_node_struct* left; /**< The left operand. */
    struct ast_node_struct* right; /**< The right operand.  */
    char* operator; /**< The operator. */
} BinaryOperation;

/**
 * @struct UnaryOperation
 * @brief Represents a unary operation. 
 * 
 * Nodes passed into returns or prints must be factor nodes so we can ensure proper data type. 
 * 
 * Current operations include:
 * - Arithmetic Negation: -(numerical expression)
 * - Logical Negation: not (boolean expression)
 * - Return Statements: return(node)
 * - Print Statenments: print(node)
 */

typedef struct {
    struct ast_node_struct* operand; /**< The node operand. */
    DataType type; /**< The operand type (necessary for prints and returns, and for specifying arithemtic or logical negation) */
} UnaryOperation;

//TERMINAL NODES: 

/**
 * @struct Parameter
 * @brief Encodes a parameter from a function declaration.
 */

typedef struct {
    DataType parameter_type; /**< The parameter type. */
    char* parameter_name; /**< The paramete rname */
} Parameter;

/**
 * @struct Variable
 * @brief Encodes a variable
 */

typedef struct {
    char* variable_name; /**< Variable name. */
} Variable;

/**
 * @struct NumberLiteral
 * @brief Represents a number. 
 * 
 * Also used to encode booleans.  
 */

typedef struct {
    int value; /**< The number value. */
} NumberLiteral;

/**
 * @struct StringLiteral
 * @brief Represents a string literal. 
 */

typedef struct {
    char* value; /**< The string. */
    int id; /**< The string id. */
} StringLiteral;

/**
 * @union ASTSpecialization
 * @brief A union so a node can choose the data it needs to hold based on its type. 
 */

typedef union {
    Block block;
    FunctionDeclaration func_dec;
    FunctionCall func_call;
    IfStatement if_statement;
    WhileLoop while_loop;
    VariableDeclaration var_dec;
    VariableAssignment var_assign;
    BinaryOperation binary_op;
    UnaryOperation print_statement;
    UnaryOperation return_statement;
    UnaryOperation negation;
    Parameter param;
    Variable var;
    NumberLiteral num;
    StringLiteral string;
} ASTSpecialization;

/**
 * @brief A general node definition. 
 * 
 * Utilizes the union to be memory efficient. Node only utilizes memory needed for its specialization.
 * Each node has a pointer to the scope it lives in.
 */

typedef struct ast_node_struct {
    NodeType type; /**< The type of node. */
    ASTSpecialization specialization; /**< Its specialization from the union. */
    Scope* scope; /**< The scope where this node lives. */
} ASTNode;

/**
 * @brief Creates and initalizes a new node. 
 * 
 * @param type The node type. 
 * @param scope The scope it lives in. 
 * @return Pointer to the newly created node. 
 */

ASTNode* init_node(NodeType type, Scope* scope);

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
 */

void free_node(ASTNode* node);

/**
 * @brief Wrapper for the free node function. 
 * 
 * @param node Pointer to the node. 
 */

void free_node_wrapper(void* node);

#endif
