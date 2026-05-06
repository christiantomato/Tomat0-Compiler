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

    //Testing...

    //make some symbols
    Symbol* sym1 = init_symbol("counter", SYMBOL_VARIABLE);
    Symbol* sym2 = init_symbol("calculate", SYMBOL_FUNCTION);
    Symbol* sym3 = init_symbol("foo", SYMBOL_VARIABLE);

    //make a hash table
    HashTable* table = init_hash_table(5);

    //add the symbols
    hash_put(table, sym1);
    hash_put(table, sym2);
    hash_put(table, sym3);

    //remove
    hash_remove(table, sym1->name);

    //get one
    Symbol* mine = hash_get(table, sym2->name);
    printf("%s", symbol_to_str(mine));

    //lets see it
    FILE* symbols_file = fopen("output/symbols_output.csv", "w");
    hash_print(symbols_file, table);
    fclose(symbols_file);

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
