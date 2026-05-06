/**
 * @file main.c
 * @brief Goes through the compilation pipeline. 
 * 
 * Lexical Analysis -> Syntactic Analysis -> Code Generation
 */

#include "include/main.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    /*
     * LEXICAL ANALYSIS
     */

    //read tomat0 source code from file
    char* file_contents = read_file("main.tmt");

    //initalize the lexer with contents, tokenize and store all to a list
    Lexer* my_lexer = init_lexer(file_contents);
    List* tokens_list = tokenize_all(my_lexer);

    //write all the tokens to a file
    FILE* tokens_file = fopen("output/tokens_output.csv", "w");
    print_list(tokens_file, tokens_list, token_to_str);
    fclose(tokens_file);

    //free the lexer, it has done its job
    free_lexer(my_lexer);

    /*
     * SYNTACTIC ANALYSIS
     */

    //intialize the parser and symbol table
    Parser* my_parser = init_parser(tokens_list);

    //parse everything and return the root node
    ASTNode* ast_root = parser_parse(my_parser);

    //write ast representation from root node now
    FILE* ast_file = fopen("output/ast_output.txt", "w");
    print_ast(ast_file, ast_root, 0);
    fclose(ast_file);

    //we can free the tokens and parser now
    free_list(tokens_list, free_token_wrapper);
    free_parser(my_parser);

    /*
     * CODE GENERATION
     */
    
    return 0;
}
