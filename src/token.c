#include "include/token.h"
#include <stdlib.h>

/*
Initialize Token Function

creates a token with the given type and value

TokenType type: the type of token it will be classified as
char* value: the value the token will hold

return: returns the created token pointer
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
Token Type As String Function

converts the token type enum into a string

Token* token: the target token for which we are finding the type

return: returns the token type, as a string pointer
*/

char* token_type_as_str(Token* token) {
    switch(token->type) {
        case TOKEN_KEYWORD_INT: return "TOKEN_KEYWORD_INT"; break;
        case TOKEN_KEYWORD_STRING: return "TOKEN_KEYWORD_STRING"; break;
        case TOKEN_KEYWORD_SOUT: return "TOKEN_KEYWORD_SOUT"; break;
        case TOKEN_KEYWORD_IF: return "TOKEN_KEYWORD_IF"; break;
        case TOKEN_KEYWORD_WHILE: return "TOKEN_KEYWORD_WHILE"; break;
        case TOKEN_ID: return "TOKEN_ID"; break;
        case TOKEN_STRING: return "TOKEN_STRING"; break;
        case TOKEN_NEWLINE: return "TOKEN_NEWLINE"; break;
        case TOKEN_NUM: return "TOKEN_NUM"; break;
        case TOKEN_EQUALS: return "TOKEN_EQUALS"; break;
        case TOKEN_SEMI: return "TOKEN_SEMI"; break;
        case TOKEN_COMMA: return "TOKEN_COMMA"; break;
        case TOKEN_UNDERSCORE: return "TOKEN_UNDERSCORE"; break;
        case TOKEN_LPAREN: return "TOKEN_LPAREN"; break;
        case TOKEN_RPAREN: return "TOKEN_RPAREN"; break;
        case TOKEN_LCHEVRON: return "TOKEN_LCHEVRON"; break;
        case TOKEN_RCHEVRON: return "TOKEN_RCHEVRON"; break;
        case TOKEN_FSLASH: return "TOKEN_FSLASH"; break;
        case TOKEN_BSLASH: return "TOKEN_BSLASH"; break;
        case TOKEN_PLUS: return "TOKEN_PLUS"; break;
        case TOKEN_HYPHEN: return "TOKEN_HYPHEN"; break;
        case TOKEN_ASTERIK: return "TOKEN_ASTERIK"; break;
        case TOKEN_QUESTION: return "TOKEN_QUESTION"; break;
        case TOKEN_EOF: return "TOKEN_EOF"; break;
        default: return "TOKEN_UNKNOWN"; break;
    }
}

/*
Free Token Function

frees all allocated memory of a token struct

Token* token: the token pointer to be freed

return: returns 0 for success, 1 if failure
*/

int free_token(Token* token) {
    //ensure the token isn't garbage
    if(token == NULL) {
        return 1;
    }
    //free the pointer to the value of the token
    free(token->value);
    //free the token itself
    free(token);
    return 0;
}

/*
Free Token Wrapper Function

a wrapper function for the free token function, utilized with my built-in array list 
to ensure i can free a list of tokens properly

void* token: the token to be freed
*/

void free_token_wrapper(void* token) {
    free_token((Token*) token);
}
