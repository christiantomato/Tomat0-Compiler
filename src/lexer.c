/**
 * @file lexer.c
 * @brief Implementation of the tokenization algorithm.
 */

#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Creates a lexer and passes in the contents for it to analyze.
 */

Lexer* init_lexer(char* contents) {
    //allocate memory for the lexer
    Lexer* lexer = malloc(sizeof(Lexer));
    //set up initial values
    lexer->index = 0;
    lexer->curr = contents[lexer->index];
    lexer->contents = contents;
    return lexer;
}

/**
 * @brief Advances to the next character in the contents. 
 * 
 * @param lexer Pointer to the lexer. 
 */

static void lexer_advance(Lexer* lexer) {
    //if we are still in string and not at null character
    if(lexer->curr != '\0' && lexer->index < strlen(lexer->contents)) {
        //increment the index
        lexer->index++;
        //move to the next character in the contents
        lexer->curr = lexer->contents[lexer->index];
    }
}

/**
 * @brief Skips any blank space between characters. 
 * 
 * All blank spaces are irrelevant for lexical analysis except new lines, since 
 * the language uses them as delimeters. 
 * 
 * @param lexer Pointer to the lexer. 
 */

static void lexer_skip_whitespace(Lexer* lexer) {
    //skip blank space - but not newlines!
    while(isspace(lexer->curr) && lexer->curr != '\n') {
        lexer_advance(lexer);
    }
}

/**
 * @brief Returns the current lexer character as a string. 
 * 
 * Used for saving the value of single characters. 
 *
 * @param lexer Pointer to the lexer. 
 * @return Null terminated, single char string so lexer can process. 
 */

static char* lexer_char_as_str(Lexer* lexer) {
    //special case for a newline character
    if(lexer->curr == '\n') {
        //return it in the form "\n"
        return strdup("\\n");
    }
    //lexer->c is char, we need to make it a proper null terminated string
    char* char_as_str = malloc(2*sizeof(char));
    //build the null terminated string
    char_as_str[0] = lexer->curr;
    char_as_str[1] = '\0';
    return char_as_str;
}

/**
 * @brief Continues tokenization and returns inputted token.
 * 
 * Helper function, useful for single characters in the big switch case.
 * Returns the current token processed so it can be propagated out, and advances lexer to get ready for next. 
 * 
 * @param lexer Pointer to the lexer. 
 * @param token Pointer to the token to return. 
 * 
 * @return Pointer to the current token being tokenized. 
 */

static Token* continue_with_token(Lexer* lexer, Token* token) {
    //advance with the token
    lexer_advance(lexer);
    return token;
}

/**
 * @brief Tokenizes a string. 
 * 
 * Responsible for obtaining the value of the string once it has been recognized
 * that a string is needed to be tokenized, continuting through the characters until the
 * closing quotation has been reached. 
 * 
 * @param lexer Pointer to the lexer. 
 * @return Pointer to the string token. 
 */

static Token* tokenize_string(Lexer* lexer) {
    //skip the initial quotation
    lexer_advance(lexer);
    //allocate memory for the string we will be returning
    char* str_value = malloc(sizeof(char));
    //make sure no garbage values, set null character
    str_value[0] = '\0';
    //build the string as long as we have not reached next quotation
    while(lexer->curr != '"') {
        //obtain the current lexer character as a string, and start building the entire string one character at a time
        char* temp = lexer_char_as_str(lexer);
        //reallocate memory as needed (+1 for the newly added character, and +1 for the null terminator which isn't counted)
        str_value = realloc(str_value, (strlen(str_value) + 2) * sizeof(char));
        //concatenate it to our string
        strcat(str_value, temp);
        //free the temporary character as it has been concatenated and is not needed
        free(temp);
        //advance to next character in sequence
        lexer_advance(lexer);
    }
    //ignore closing quote
    lexer_advance(lexer);
    return init_token(TOKEN_STRING, str_value);
}

/**
 * @brief Tokenizes an identifier. 
 * 
 * Responsible for obtaining the value of the identifier. These could be: 
 * keywords, function names, variables names, etc. 
 * Continues to build the value as long as the identifier is alphabetical. 
 * 
 * Therefore, the language does not allow:
 * snake_case or numbers in naming (the convention is camel case). 
 * 
 * @param lexer Pointer to the lexer. 
 * @return Pointer to the identifier token. 
 */

static Token* tokenize_ID(Lexer* lexer) {
    char* id_value = malloc(sizeof(char));
    id_value[0] = '\0';
    //build the string as long as our identifier is alphabetical 
    while(isalpha(lexer->curr)) {
        char* temp = lexer_char_as_str(lexer);
        id_value = realloc(id_value, (strlen(id_value) + 2) * sizeof(char));
        strcat(id_value, temp);
        free(temp);
        lexer_advance(lexer);
    }

    //checks if our identifier is a keyword
    if(strcmp(id_value, "int") == 0) {
        return init_token(TOKEN_KEYWORD_INT, id_value);
    }
    else if(strcmp(id_value, "string") == 0) {
        return init_token(TOKEN_KEYWORD_STRING, id_value);
    }
    else if(strcmp(id_value, "sout") == 0) {
        return init_token(TOKEN_KEYWORD_SOUT, id_value);
    }
    else if(strcmp(id_value, "if") == 0) {
        return init_token(TOKEN_KEYWORD_IF, id_value);
    }
    else if(strcmp(id_value, "while") == 0) {
        return init_token(TOKEN_KEYWORD_WHILE, id_value);
    }
    else {
        //return as a regular identifier (for function or variable names)
        return init_token(TOKEN_ID, id_value);
    }
}

/**
 * @brief Tokenizes a number. 
 * 
 * Creates a number token for any numeric characters, stores as a string (converts later).
 * 
 * @param lexer Pointer to the lexer. 
 * @return Pointer to the number token. 
 */

static Token* tokenize_number(Lexer* lexer) {
    char* strValue = malloc(sizeof(char));
    strValue[0] = '\0';
    //build the string as long as we are still on a number
    while(isdigit(lexer->curr)) {
        char* temp = lexer_char_as_str(lexer);
        strValue = realloc(strValue, (strlen(strValue) + 2) * sizeof(char));
        strcat(strValue, temp);
        free(temp);
        lexer_advance(lexer);
    }
    return init_token(TOKEN_NUM, strValue);
}

/**
 * @brief The main function that is responsible for deciding the type of token encountered.
 * 
 * Examples: 
 * "hello world" (beginning with the quotation immediately indicates a string).
 * 123 (begininning with a numeric character immediately indicates an integer).
 * myVariable (beginning with an alphabetical character immediately indicates an identifier (keyword or name)).
 * + (single characters are easy to tokenize).
 * 
 * Notice the restrictions this makes, like variable names not being able to start with numbers. 
 * 
 * @param lexer Pointer to the lexer. 
 * @return Pointer to the created token. 
 */

static Token* tokenize_next(Lexer* lexer) {
    //while we are not on the null terminating character and are still within the contents
    while(lexer->curr != '\0' && lexer->index < strlen(lexer->contents)) {
        lexer_skip_whitespace(lexer);

        if(isdigit(lexer->curr)) {
            return tokenize_number(lexer);
        }

        if(isalpha(lexer->curr)) {
            return tokenize_ID(lexer);
        }
        
        switch(lexer->curr) {
            case '"': return tokenize_string(lexer); break;
            //singular characters
            case '\n': return continue_with_token(lexer, init_token(TOKEN_NEWLINE, lexer_char_as_str(lexer))); break;
            case '=': return continue_with_token(lexer, init_token(TOKEN_EQUALS, lexer_char_as_str(lexer))); break;
            case ';': return continue_with_token(lexer, init_token(TOKEN_SEMI, lexer_char_as_str(lexer))); break;
            case ',': return continue_with_token(lexer, init_token(TOKEN_COMMA, lexer_char_as_str(lexer))); break;
            case '.': return continue_with_token(lexer, init_token(TOKEN_PERIOD, lexer_char_as_str(lexer))); break;
            case '\'': return continue_with_token(lexer, init_token(TOKEN_APOSTROPHE, lexer_char_as_str(lexer))); break;
            case '_': return continue_with_token(lexer, init_token(TOKEN_UNDERSCORE, lexer_char_as_str(lexer))); break;
            case '(': return continue_with_token(lexer, init_token(TOKEN_LPAREN, lexer_char_as_str(lexer))); break;
            case ')': return continue_with_token(lexer, init_token(TOKEN_RPAREN, lexer_char_as_str(lexer))); break;
            case '<': return continue_with_token(lexer, init_token(TOKEN_LCHEVRON, lexer_char_as_str(lexer))); break;
            case '>': return continue_with_token(lexer, init_token(TOKEN_RCHEVRON, lexer_char_as_str(lexer))); break;
            case '/': return continue_with_token(lexer, init_token(TOKEN_FSLASH, lexer_char_as_str(lexer))); break;
            case '\\': return continue_with_token(lexer, init_token(TOKEN_BSLASH, lexer_char_as_str(lexer))); break;
            case '+': return continue_with_token(lexer, init_token(TOKEN_PLUS, lexer_char_as_str(lexer))); break;
            case '-': return continue_with_token(lexer, init_token(TOKEN_HYPHEN, lexer_char_as_str(lexer))); break;
            case '*': return continue_with_token(lexer, init_token(TOKEN_ASTERISK, lexer_char_as_str(lexer))); break;
            case '?': return continue_with_token(lexer, init_token(TOKEN_QUESTION, lexer_char_as_str(lexer))); break;
            default: return NULL;
        }
    }
    //return null if nothing is left to tokenize
    return NULL;
}

/*
 * Loops through with the tokenize next function to create a list of tokens. 
 */

List* tokenize_all(Lexer* lexer) {
    //create the list we are going to return, roughly estimate size needed
    List* tokens_list = init_list(50);

    //tokenize the contents
    Token* token;
    while((token = tokenize_next(lexer)) != NULL) {
        //add to the list of tokens
        list_add(tokens_list, token);
    }

    //add the end of file token to the list once done tokenzing
    list_add(tokens_list, init_token(TOKEN_EOF, strdup("eof")));

    return tokens_list;
}

/*
 * Frees all memory allocated by the lexer struct.
 */

int free_lexer(Lexer* lexer) {
    //ensure the lexer isn't garbage
    if(lexer == NULL) {
        return 1;
    }
    //free the pointer to the contents
    free(lexer->contents);
    //free the lexer itself
    free(lexer);
    return 0;
}
