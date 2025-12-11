/*
Lexer Structure

responsible for conducting lexical analysis
the lexer will traverse through the contents of the .tmt file and "tokenize" the contents,
acting as the first step in breaking up our contents into meaningful units

ex.
int x = 10
tokenized:
TOKEN_KEYWORD_INT, TOKEN_ID, TOKEN_EQUALS, TOKEN_NUM

char* contents: the actual contents we are performing lexical analysis on
unsigned int index: the index of the string contents, acting as the cursor throughout the file
char c: the current character the lexer is on in the file contents
*/

#include "token.h"
#include "array_list.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct lexer_struct {
    char* contents;
    unsigned int index;
    char curr;
} Lexer;

//creates a lexer
Lexer* init_lexer(char* contents);
//moves to next character
void lexer_advance(Lexer* lexer);
//skips unnecessary whitespace
void lexer_skip_whitespace(Lexer* lexer);
//tokenize everything and put it into a list
List* tokenize_all(Lexer* lexer);
//to get the next token
Token* tokenize_next(Lexer* lexer);
//tokenizes a string
Token* tokenize_string(Lexer* lexer); 
//tokenizes words, like variables and keywords (different from strings)
Token* tokenize_ID(Lexer* lexer);
//to tokenize numbers 
Token* tokenize_number(Lexer* lexer);
//advance and prepare for next tokenization
Token* continue_with_token(Lexer* lexer, Token* token);
//to get the current character of the lexer
char* lexer_char_as_str(Lexer* lexer);
//frees allocated lexer memory
int free_lexer(Lexer* lexer);

#endif
