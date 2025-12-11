#include "include/symbol_table.h"
#include <stdlib.h>
#include <string.h>

/*
Initialize Table Function

initializes the symbol table data structure

return: pointer to the table
*/

SymbolTable* init_symbol_table() {
    //allocate memory for the symbol table 
    SymbolTable* table = malloc(sizeof(SymbolTable));
    //set up symbols array with some reasonable initial capacity
    table->symbols = init_list(10);
    //start at 0, allocate 8 bytes for each variable once added
    table->current_offset = 0;
    return table;
}

/*
Add To Table Function

stores a symbol in the memory of the symbol table

SymbolTable* table: the table we are adding the symbol to 
const char* name: name of the variable or function identifier
const char* type: data type of identifier
*/

void add_to_table(SymbolTable* table, char* name, const char* type) {
    //create the symbol and add it to the list
    Symbol* new_symbol = malloc(sizeof(Symbol));
    new_symbol->name = name;
    new_symbol->type = type;
    //allocate 8 bytes for the new symbol
    new_symbol->memory_offset = table->current_offset - 8;
    //update current offset for table
    table->current_offset = new_symbol->memory_offset;
    //add to the list
    list_add(table->symbols, new_symbol);
}

/*
Look Up Symbol Function

performs a linear search to look up the specified symbol

SymbolTable* table: the table in which we are looking for the symbol
const char* name: name of symbol we are looking for

return: pointer to the symbol
*/

Symbol* look_up_symbol(SymbolTable* table, char* name) {
    //do a linear search
    for(int i = 0; i < table->symbols->num_items; i++) {
        Symbol* curr_symbol = table->symbols->array[i];
        if(strcmp(curr_symbol->name, name) == 0) {
            return curr_symbol;
        }
    }
    //nothing found
    return NULL;
}

/*
Free Symbol Function

frees the allocated memory of a symbol

Symbol* symbol: the symbol being freed

return: 0 for success, 1 otherwise
*/

int free_symbol(Symbol* symbol) {
    //make sure symbol isn't garbage
    if(symbol == NULL) {
        return 1;
    }
    //free dynamically allocated things
    free(symbol->name);
    free(symbol);
    return 0;
}


/*
Free Symbol Wrapper Function

wrapper function to pass into array list to free the symbols

void* symbol: the symbol being freed
*/

void free_symbol_wrapper(void* symbol) {
    free_symbol((Symbol*) symbol);
}


/*
Free Symbol Table Function

frees the allocated memory of a symbol table

SymbolTable* table: the table being freed

return: 0 for success, 1 otherwise
*/

int free_symbol_table(SymbolTable* table) {
    //not garbage
    if(table == NULL) {
        return 1;
    }
    //free the list
    free_complex_list(table->symbols, free_symbol_wrapper);
    //free table
    free(table);
    return 0;
}
