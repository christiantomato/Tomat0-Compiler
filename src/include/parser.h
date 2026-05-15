/**
 * @file parser.h
 * @brief Parses a list of tokens and generates an abstract syntax tree. 
 * 
 * Responsible for conducting syntactic analysis.
 * The parser makes sense of the tokens provided by the lexer,
 * and puts everything into a tree which describes the sequence in which the program is executed.
 * Uses a LL(1) parsing design. 
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast_node.h"
#include "program.h"

/**
 * @struct Parser
 * @brief The parser structure which parses tokens and builds the ast. 
 */

typedef struct {
    List* tokens; /**< List of tokens that are passed in, to be parsed. */
    Token* current_token; /**< Reference to the current token being processed. */
    unsigned int index; /**< Index position for the tokens list. */
    ASTNode* root; /**< Reference to the root node of the ast. */
    Scope* current_scope; /**< Reference to the current scope we are in. */
    List* strings; /**< Reference to all strings that are created in the program. */
    List* scopes; /**< Reference to all scopes that are created in the program. */
} Parser;

/**
 * @brief Creates and initializes a parser.
 * 
 * @param tokens The list of tokens to parse. 
 * @return Pointer to the parser. 
 */

Parser* init_parser(List* tokens);

/**
 * @brief Main parsing function. 
 * 
 * Parses through the entire tokens list and keeps track of declared variables with symbol tables. 
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the program struct representation.
 */

Program* parser_parse(Parser* parser);

/**
 * @brief Frees allocated memory by the parser. 
 * 
 * @param parser Pointer to the parser. 
 */

void free_parser(Parser* parser);

#endif