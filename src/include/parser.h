/**
 * @file parser.h
 * @brief Parses a list of tokens and generates an abstract syntax tree. 
 * 
 * Responsible for conducting syntactic analysis.
 * The parser makes sense of the tokens provided by the lexer,
 * and puts everything into a tree which describes the sequence in which the program is executed.
 * Uses a LL(1) parsing design. 
 */

#include "token.h"
#include "ast_node.h"

#ifndef PARSER_H
#define PARSER_H

/**
 * @struct Parser
 * @brief The parser structure which parses tokens. 
 */

typedef struct {
    ASTNode* root; /**< Reference to the root node. */
    List* tokens; /**< List of tokens being parsed. */
    Token* current_token; /**< Reference to the current token being processed. */
    unsigned int index; /**< Index position for the tokens list. */
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
 * Parses through the entire tokens list and keeps track of declared variables with symbol table. 
 * 
 * @param parser Pointer to the parser.
 * @param table Pointer to the symbol table. 
 * @return Pointer to the root node of the parser.
 */

ASTNode* parser_parse(Parser* parser);

/**
 * @brief Frees allocated memory by the parser. 
 * 
 * @param parser Pointer to the parser. 
 */

void free_parser(Parser* parser);

#endif