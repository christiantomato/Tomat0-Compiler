/**
 * @file token.h
 * @brief Describes all types of tokens that can be parsed. 
 *
 * Defines the different types of tokens that our lexer will encounter.
 * These include:
 * - numbers (ex. 123)
 * - strings (ex. "hello world")
 * - keywords (ex. sout, int, if, while)
 * - punctuation/symbols (ex. (), >, =, +)
 */

#ifndef TOKEN_H
#define TOKEN_H

/**
 * @enum TokenType
 * @brief Represents all possible token types produced by the lexer. 
 * 
 * This enumeration defines the different categories of tokens
 * recognized in the Tomat0 language, including keywords, identifiers,
 * literals, operators, and punctuation.
 */

typedef enum {
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
    TOKEN_PERIOD,
    TOKEN_APOSTROPHE,
    TOKEN_UNDERSCORE,
    TOKEN_LPAREN,
    TOKEN_RPAREN, 
    TOKEN_LCHEVRON,
    TOKEN_RCHEVRON,
    TOKEN_FSLASH,
    TOKEN_BSLASH,
    TOKEN_PLUS,
    TOKEN_HYPHEN,
    TOKEN_ASTERISK,
    TOKEN_QUESTION,
    TOKEN_EOF
} TokenType;

/**
 * @struct Token
 * @brief Encodes necessary information for each token. 
 */

typedef struct {
    TokenType type; /**< The type of token it is. */
    char* value; /**< The actual data of the token.  */
} Token; 

/**
 * @brief Creates and initializes a new token. 
 * 
 * @param type The token type. 
 * @param value The token data. 
 * @return Token* Pointer to the created token. 
 */

Token* init_token(TokenType type, char* value);

/**
 * @brief Returns the enum name for the token as a string. 
 * 
 * @param token Pointer to the token. 
 * @return char* String corresponding to the enum name. 
 */

char* token_type_as_str(Token* token);

/**
 * @brief ToString for a token. 
 * 
 * @param token Generic pointer which should be a token (so we can pass the ToString to other functions).
 * @return char* String representation of the token. 
 */

char* token_to_str(void* token);

/**
 * @brief Frees the allocated memory for a token. 
 * 
 * @param token Pointer to the token. 
 * @return 0 if successful, 1 otherwise. 
 */

int free_token(Token* token);

/**
 * @brief Wrapper for the free token function. 
 * 
 * @param token Generic pointer to the token. 
 */

void free_token_wrapper(void* token);

#endif
