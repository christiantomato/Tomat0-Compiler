/**
 * @file main.c
 * @brief Goes through the compilation pipeline. 
 * 
 * Lexical Analysis -> Syntactic Analysis + Semantic Analysis -> Code Generation.
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

    //intialize the parser
    Parser* my_parser = init_parser(tokens_list);

    //parse everything and return the program representation, containing ast root and scopes list.
    Program* program_representation = parser_parse(my_parser);

    //write ast representation from root node and print all scope symbol tables to files
    FILE* symbol_tables_file = fopen("output/tables_output.txt", "w");
    FILE* ast_file = fopen("output/ast_output.txt", "w");
    print_ast(ast_file, program_representation->root, 0);
    print_list(symbol_tables_file, program_representation->scopes, scope_to_str);
    fclose(ast_file);
    fclose(symbol_tables_file);

    //finally, free the tokens and parser now
    free_list(tokens_list, free_token_wrapper);
    free_parser(my_parser);

    /*
     * CODE GENERATION
     */

    //create the assembly file we will write to
    FILE* assembly_file = fopen("output/generated_asm.s", "w");
    //generate the assembly
    generate_assembly(assembly_file, program_representation);
    //close the file
    fclose(assembly_file);

    //make an executable
    system("gcc output/generated_asm.s -o tomat0executable");
    //move compiled Tomat0 file to output directory
    system("mv tomat0executable output");
    //execute
    //system("./output/tomat0executable");

    //free the program
    free_program(program_representation);
    
    return 0;
}
