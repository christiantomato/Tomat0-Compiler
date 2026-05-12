/**
 * @file lexer.h
 * @brief Tokenizes the contents of the .tmt source file. 
 * 
 * Responsible for conducting lexical analysis.
 * The lexer will traverse through the contents of the .tmt file and "tokenize" the contents,
 * acting as the first step in breaking up our contents into meaningful units that can be more easily parsed. 
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "array_list.h"

/**
 * @struct Lexer
 * @brief The lexer structure which tokenizes a string. 
 */

typedef struct {
    char* contents; /**< The string which the lexer traverses. */
    unsigned int index; /**< The current index the lexer is on while traversing. */
    char curr; /**< The current character the lexer is processing. */
} Lexer;

/**
 * @brief Creates and initializes a new lexer. 
 * 
 * @param contents The string the lexer will traverse. 
 * @return Pointer to the created lexer. 
 */

Lexer* init_lexer(char* contents);

/**
 * @brief Starts the tokenization process for the provided initalized lexer. 
 * 
 * @param lexer Pointer to the lexer which will conduct lexical analysis on its contents. 
 * @return Pointer to a list containing token pointers. 
 */

List* tokenize_all(Lexer* lexer);

/**
 * @brief Frees all allocated memory by the lexer.
 * 
 * @param lexer Pointer to the lexer. 
 */

void free_lexer(Lexer* lexer);

#endif
