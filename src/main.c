#include "include/all.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

    //get file contents with file reader
    char* file_contents = read_file("main.tmt");
    
    //display what is in main.tmt
    printf("%s\n\n", file_contents);

    //create the lexer and ready the tokens list
    Lexer* my_lexer = init_lexer(file_contents);
    List* tokens_list = init_list(30);

    //tokenize the contents
    Token* token = (void*)0;
    while((token = tokenize_next(my_lexer)) != NULL) {
        printf("Token type: %s, Token value: %s \n", token_type_as_str(token), token->value);
        //add to the list of tokens
        list_add(tokens_list, token);
    }

    //free the lexer, it has done its job
    free_lexer(my_lexer);
    printf("\n");

    //add the end of file token to the list
    Token* end_of_file_token = init_token(TOKEN_EOF, NULL);
    list_add(tokens_list, end_of_file_token);

    //bring out the parser and symbol table
    Parser* my_parser = init_parser(tokens_list);
    SymbolTable* my_table = init_table();

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

    //got to work out how we are going to free everything... (currently in shambles)
    
    //make an executable
    system("gcc output/generated_asm.s -o tomat0executable");
    //move compiled tomat0 file to output directory
    system("mv tomat0executable output");
    //execute
    system("./output/tomat0executable");
    
    //success
    return 0;
}