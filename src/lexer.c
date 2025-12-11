#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
Initialize Lexer Function

creates a lexer and passes in the contents for it to analyze

char* contents: the string of text to analyze (read from the .tmt file)

return: pointer to the created lexer
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

/*
Lexer Advance Function

advances to the next character in the contents

Lexer* lexer: the lexer that is advanced
*/

void lexer_advance(Lexer* lexer) {
    //if we are still in string and not at null character
    if(lexer->curr != '\0' && lexer->index < strlen(lexer->contents)) {
        //increment the index
        lexer->index++;
        //move to the next character in the contents
        lexer->curr = lexer->contents[lexer->index];
    }
}

/*
Lexer Skip Whitespace Function

skips any blank space between characters as they are irrelvant, 
but takes into account new lines as tomat0 uses them as delimeters

Lexer* lexer: the lexer to skip whitespace
*/

void lexer_skip_whitespace(Lexer* lexer) {
    //skip blank space - but not newlines!
    while(isspace(lexer->curr) && lexer->curr != '\n') {
        lexer_advance(lexer);
    }
}

/*
Tokenize All Function

loops through with the tokenize next function to create a list of tokens

Lexer* lexer: the lexer that is tokenizing

return: pointer to a list containing all the tokens
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

    //return the list
    return tokens_list;
}

/*
Tokenize Next Function

the main function that is responsible for deciding which type of token
we are going to get, based on the first character

ex.
"hello world" (beginning with the quotation immediately indicates a string)
69000 (begininning with a numeric character immediately indicates an integer)
myVariable (beginning with an alphabetical character immediately indicates an identifier (keyword or name))
+ (single characters are easy to tokenize)
*notice the restrictions this makes, like variable names not being able to start with numbers*

Lexer* lexer: the lexer that is tokenizing

return: the created token pointer
*/

Token* tokenize_next(Lexer* lexer) {
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
            case '_': return continue_with_token(lexer, init_token(TOKEN_UNDERSCORE, lexer_char_as_str(lexer))); break;
            case '(': return continue_with_token(lexer, init_token(TOKEN_LPAREN, lexer_char_as_str(lexer))); break;
            case ')': return continue_with_token(lexer, init_token(TOKEN_RPAREN, lexer_char_as_str(lexer))); break;
            case '<': return continue_with_token(lexer, init_token(TOKEN_LCHEVRON, lexer_char_as_str(lexer))); break;
            case '>': return continue_with_token(lexer, init_token(TOKEN_RCHEVRON, lexer_char_as_str(lexer))); break;
            case '/': return continue_with_token(lexer, init_token(TOKEN_FSLASH, lexer_char_as_str(lexer))); break;
            case '\\': return continue_with_token(lexer, init_token(TOKEN_BSLASH, lexer_char_as_str(lexer))); break;
            case '+': return continue_with_token(lexer, init_token(TOKEN_PLUS, lexer_char_as_str(lexer))); break;
            case '-': return continue_with_token(lexer, init_token(TOKEN_HYPHEN, lexer_char_as_str(lexer))); break;
            case '*': return continue_with_token(lexer, init_token(TOKEN_ASTERIK, lexer_char_as_str(lexer))); break;
            case '?': return continue_with_token(lexer, init_token(TOKEN_QUESTION, lexer_char_as_str(lexer))); break;
            default: return NULL;
        }
    }
    //return null if nothing is left to tokenize
    return NULL;
}

/*
Tokenize String Function

responsible for obtaining the value of the string once it has been 
recognized that a string is needed to be tokenized, continuing 
through the characters until the closing quotation has been reached

Lexer* lexer: the lexer that is tokenizing the string

return: the token pointer of type TOKEN_STRING
*/

Token* tokenize_string(Lexer* lexer) {
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

/*
Tokenize Identifier Function

responsible for obtaining the value of the identifier,
which could be a keyword, variable name, or function name
continues to build the value as long as the identifier is alphabetical,
therefore tomat0 does not allow snake_case or numbers in naming
uses the same string building process as tokenize_string()

Lexer* lexer: the lexer that is tokenizing the identifier

return: the token pointer of type TOKEN_ID or TOKEN_KEYWORD
*/

Token* tokenize_ID(Lexer* lexer) {
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

/*
Tokenize Number Function

creates a number token for any numeric characters, storing the number as a string

Lexer* lexer: the lexer tokenizing the numbers

return: the token pointer of type TOKEN_NUM
*/

Token* tokenize_number(Lexer* lexer) {
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

/*
Continue With Token Function

helper function to cleanly return back the token inputted, 
and advance the lexer to be ready for the next tokenization

Lexer* lexer: the lexer that will advance
Token* token: the token that will be returned

return: the inputted token pointer
*/

Token* continue_with_token(Lexer* lexer, Token* token) {
    //advance with the token
    lexer_advance(lexer);
    return token;
}

/*
Lexer Character As String Function

helper function to get the current lexer characater as a string pointer

Lexer* lexer: the lexer whose character we are requesting

return: the current character as a string pointer
*/

char* lexer_char_as_str(Lexer* lexer) {
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

/*
Free Lexer Function

frees all memory allocated my the lexer struct

Lexer* lexer: the lexer to be freed

return: 0 for success, 1 for failure
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
