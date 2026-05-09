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
        case TOKEN_KEYWORD_INT: return "TOKEN_KEYWORD_INT"; break;
        case TOKEN_KEYWORD_STRING: return "TOKEN_KEYWORD_STRING"; break;
        case TOKEN_KEYWORD_BOOL: return "TOKEN_KEYWORD_BOOL"; break;
        case TOKEN_KEYWORD_PRINT: return "TOKEN_KEYWORD_PRINT"; break;
        case TOKEN_KEYWORD_IF: return "TOKEN_KEYWORD_IF"; break;
        case TOKEN_KEYWORD_WHILE: return "TOKEN_KEYWORD_WHILE"; break;
        case TOKEN_KEYWORD_TRUE: return "TOKEN_KEYWORD_TRUE"; break;
        case TOKEN_KEYWORD_FALSE: return "TOKEN_KEYWORD_FALSE"; break;
        case TOKEN_KEYWORD_IS: return "TOKEN_KEYWORD_IS"; break;
        case TOKEN_KEYWORD_YIELDS: return "TOKEN_KEYWORD_YIELDS"; break;
        case TOKEN_KEYWORD_HARVEST: return "TOKEN_KEYWORD_HARVEST"; break;
        case TOKEN_KEYWORD_SPROUT: return "TOKEN_KEYWORD_SPROUT"; break;
        case TOKEN_KEYWORD_TOMATO: return "TOKEN_KEYWORD_TOMATO"; break;
        case TOKEN_KEYWORD_THROW: return "TOKEN_KEYWORD_THROW"; break;
        case TOKEN_ID: return "TOKEN_ID"; break;
        case TOKEN_NUM: return "TOKEN_NUM"; break;
        case TOKEN_STRING: return "TOKEN_STRING"; break;
        case TOKEN_NEWLINE: return "TOKEN_NEWLINE"; break;
        case TOKEN_TILDA: return "TOKEN_TILDA"; break;
        case TOKEN_EQUALS: return "TOKEN_EQUALS"; break;
        case TOKEN_SEMI: return "TOKEN_SEMI"; break;
        case TOKEN_COMMA: return "TOKEN_COMMA"; break;
        case TOKEN_PERIOD: return "TOKEN_PERIOD"; break;
        case TOKEN_APOSTROPHE: return "TOKEN_APOSTROPHE"; break;
        case TOKEN_UNDERSCORE: return "TOKEN_UNDERSCORE"; break;
        case TOKEN_LPAREN: return "TOKEN_LPAREN"; break;
        case TOKEN_RPAREN: return "TOKEN_RPAREN"; break;
        case TOKEN_LCURLY: return "TOKEN_LCURLY"; break;
        case TOKEN_RCURLY: return "TOKEN_RCURLY"; break;
        case TOKEN_LCHEVRON: return "TOKEN_LCHEVRON"; break;
        case TOKEN_RCHEVRON: return "TOKEN_RCHEVRON"; break;
        case TOKEN_FSLASH: return "TOKEN_FSLASH"; break;
        case TOKEN_BSLASH: return "TOKEN_BSLASH"; break;
        case TOKEN_PLUS: return "TOKEN_PLUS"; break;
        case TOKEN_HYPHEN: return "TOKEN_HYPHEN"; break;
        case TOKEN_ASTERISK: return "TOKEN_ASTERISK"; break;
        case TOKEN_QUESTION: return "TOKEN_QUESTION"; break;
        case TOKEN_EOF: return "TOKEN_EOF"; break;
        default: return "TOKEN_UNKNOWN"; break;
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
