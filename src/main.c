#include "include/main.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    /*
    LEXICAL ANALYSIS
    */

    //read tomat0 code from file
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
    SYNTACTIC ANALYSIS
    */

    //intialize the parser and symbol table
    Parser* my_parser = init_parser(tokens_list);
    SymbolTable* my_table = init_symbol_table();

    //parse everything
    parser_parse(my_parser, my_table);
     //get a reference to the root node
    ASTNode* ast_tree_root = my_parser->root;

    //write ast representation from root now
    FILE* ast_file = fopen("output/ast_output.txt", "w");
    print_ast(ast_file, ast_tree_root, 0);
    fclose(ast_file);

    //generate the assembly code
    FILE* assembly_file = fopen("output/generated_asm.s", "w");
    generate_assembly(assembly_file, ast_tree_root, my_table);
    //close the file
    fclose(assembly_file);

    //TODO: free all used memory like tokens, nodes, parser? 
    
    //make an executable
    system("gcc output/generated_asm.s -o tomat0executable");
    //move compiled tomat0 file to output directory
    system("mv tomat0executable output");
    //execute
    system("./output/tomat0executable");
    
    return 0;
}