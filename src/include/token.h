/*
Token Structure

defines the different types of tokens that our lexer will encounter
these include:
- numbers (ex. 123)
- strings (ex. "hello world")
- keywords (ex. sout, int, if, while)
- punctuation/symbols (ex. (), >, =, +)

TokenType type: the type of token (integer, symbol, keyword)
char* value: the actual value the token holds (5, +, sout)
*/

#ifndef TOKEN_H
#define TOKEN_H

typedef enum token_type {
    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_STRING,
    TOKEN_KEYWORD_SOUT,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_WHILE,
    TOKEN_ID,
    TOKEN_STRING,
    TOKEN_NEWLINE,
    TOKEN_NUM,
    TOKEN_EQUALS,
    TOKEN_SEMI,
    TOKEN_COMMA,
    TOKEN_UNDERSCORE,
    TOKEN_LPAREN,
    TOKEN_RPAREN, 
    TOKEN_LCHEVRON,
    TOKEN_RCHEVRON,
    TOKEN_FSLASH,
    TOKEN_BSLASH,
    TOKEN_PLUS,
    TOKEN_HYPHEN,
    TOKEN_ASTERIK,
    TOKEN_QUESTION,
    TOKEN_EOF
} TokenType;

typedef struct token_struct {
    TokenType type;
    char* value;
} Token; 

//for creating tokens
Token* init_token(TokenType type, char* value);
//returning the enum name for the type of token as a string
char* token_type_as_str(Token* token);
//performs a to string of token type and value
char* token_to_str(void* token);
//for freeing the memory of a token
int free_token(Token* token);
//wrapper function so we can free tokens stored in a built-in array list
void free_token_wrapper(void* token);

#endif
