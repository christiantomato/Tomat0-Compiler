/*
Parser Structure

responsible for conducting syntactic analysis
the parser makes sense of the tokens provided by the lexer,
and puts everything into a tree which describes the sequence in which the program is executed
this parser uses a top-down recursive descent LL(1) parsing method

ASTNode* root: the root node of the program, where execution starts (called the program node)
List* tokens: the tokens that were created by the lexer, which the parser must make sense of
Token* current_token: variable that keeps track of the current token the parser is parsing during algorithms
unsigned int index: variable to help traverse through the tokens list during parsing algorithms
*/

#include "token.h"
#include "ast_node.h"
#include "symbol_table.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct parser_struct {
    ASTNode* root;
    List* tokens;
    Token* current_token;
    unsigned int index;
} Parser;

//create parser
Parser* init_parser(List* tokens);
//moves to next token
void parser_advance(Parser* parser);
//looks ahead
Token* parser_peek(Parser* parser, int ahead);
//skips unecessary grammar
void parser_skip(Parser* parser);
//main parsing function which also builds the symbol table
void parser_parse(Parser* parser, SymbolTable* table);

/*
parsing functions (top down parsing)
*/

ASTNode* parse_line(Parser* parser);
ASTNode* parse_variable_declaration(Parser* parser);
ASTNode* parse_variable_assignment(Parser* parser);
ASTNode* parse_print_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);
ASTNode* parse_factor(Parser* parser);

//free allocated parser memory
int free_parser(Parser* parser);

#endif