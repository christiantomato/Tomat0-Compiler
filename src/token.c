/**
 * @file token.c
 * @brief Implements defined functions for my token definition.
 */

#include "include/token.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Creates a token with the given type and value.
 */

Token* init_token(TokenType type, char* value) {
    //allocate space for the token
    Token* token = malloc(sizeof(Token));
    //initialize and return
    token->type = type;
    token->value = value;
    return token;  
}

/*
 * Converts the token type enum into a string.
 */

char* token_type_as_str(Token* token) {
    switch(token->type) {
        case TOKEN_KEYWORD_INT: return "TOKEN_KEYWORD_INT"; 
        case TOKEN_KEYWORD_STRING: return "TOKEN_KEYWORD_STRING"; 
        case TOKEN_KEYWORD_BOOL: return "TOKEN_KEYWORD_BOOL"; 
        case TOKEN_KEYWORD_FUNC: return "TOKEN_KEYWORD_FUNC"; 
        case TOKEN_KEYWORD_PRINT: return "TOKEN_KEYWORD_PRINT"; 
        case TOKEN_KEYWORD_WHILE: return "TOKEN_KEYWORD_WHILE"; 
        case TOKEN_KEYWORD_IF: return "TOKEN_KEYWORD_IF"; 
        case TOKEN_KEYWORD_ELSE: return "TOKEN_KEYWORD_ELSE"; 
        case TOKEN_KEYWORD_NOT: return "TOKEN_KEYWORD_NOT"; 
        case TOKEN_KEYWORD_AND: return "TOKEN_KEYWORD_AND"; 
        case TOKEN_KEYWORD_OR: return "TOKEN_KEYWORD_OR"; 
        case TOKEN_KEYWORD_TRUE: return "TOKEN_KEYWORD_TRUE"; 
        case TOKEN_KEYWORD_FALSE: return "TOKEN_KEYWORD_FALSE"; 
        case TOKEN_KEYWORD_EQUALS: return "TOKEN_KEYWORD_EQUALS"; 
        case TOKEN_KEYWORD_YIELDS: return "TOKEN_KEYWORD_YIELDS"; 
        case TOKEN_KEYWORD_HARVEST: return "TOKEN_KEYWORD_HARVEST"; 
        case TOKEN_KEYWORD_SPROUT: return "TOKEN_KEYWORD_SPROUT"; 
        case TOKEN_KEYWORD_TOMATO: return "TOKEN_KEYWORD_TOMATO"; 
        case TOKEN_KEYWORD_THROW: return "TOKEN_KEYWORD_THROW"; 
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_NUM: return "TOKEN_NUM"; 
        case TOKEN_STRING: return "TOKEN_STRING"; 
        case TOKEN_NEWLINE: return "TOKEN_NEWLINE"; 
        case TOKEN_TILDA: return "TOKEN_TILDA"; 
        case TOKEN_EQUALS: return "TOKEN_EQUALS"; 
        case TOKEN_SEMI: return "TOKEN_SEMI"; 
        case TOKEN_COMMA: return "TOKEN_COMMA"; 
        case TOKEN_PERIOD: return "TOKEN_PERIOD"; 
        case TOKEN_APOSTROPHE: return "TOKEN_APOSTROPHE"; 
        case TOKEN_UNDERSCORE: return "TOKEN_UNDERSCORE"; 
        case TOKEN_LPAREN: return "TOKEN_LPAREN"; 
        case TOKEN_RPAREN: return "TOKEN_RPAREN"; 
        case TOKEN_LCURLY: return "TOKEN_LCURLY"; 
        case TOKEN_RCURLY: return "TOKEN_RCURLY"; 
        case TOKEN_LCHEVRON: return "TOKEN_LCHEVRON"; 
        case TOKEN_RCHEVRON: return "TOKEN_RCHEVRON"; 
        case TOKEN_FSLASH: return "TOKEN_FSLASH"; 
        case TOKEN_BSLASH: return "TOKEN_BSLASH"; 
        case TOKEN_PLUS: return "TOKEN_PLUS"; 
        case TOKEN_HYPHEN: return "TOKEN_HYPHEN"; 
        case TOKEN_ASTERISK: return "TOKEN_ASTERISK"; 
        case TOKEN_QUESTION: return "TOKEN_QUESTION"; 
        case TOKEN_EXCLAMATION: return "TOKEN_EXCLAMATION"; 
        case TOKEN_EOF: return "TOKEN_EOF"; 
        default: return "TOKEN_UNKNOWN"; 
    }
}

/*
 * Takes a token and formats it with type and value.
 * Uses a void pointer so we can pass the function to print_list.
 */

char* token_to_str(void* token) {
    Token* the_token = (Token*) token;
    //the string to return 
    char* token_str;

    //figure out length needed and allocate space for it
    int length = snprintf(NULL, 0, "TOKEN TYPE: %s, TOKEN VALUE: %s", token_type_as_str(the_token), the_token->value);
    token_str = malloc(length + 1);

    //actually write it this time
    snprintf(token_str, length + 1, "TOKEN TYPE: %s, TOKEN VALUE: %s", token_type_as_str(the_token), the_token->value);
    return token_str;
}

/*
 * Frees all allocated memory of a token struct.
 */

void free_token(Token* token) {
    //free the pointer to the value of the token
    free(token->value);
    //free the token itself
    free(token);
}

/*
 * A wrapper function for the free token function, utilized with my built-in array list 
 * to ensure i can free a list of tokens properly.
 */

void free_token_wrapper(void* token) {
    free_token((Token*) token);
}
