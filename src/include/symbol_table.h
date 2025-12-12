/*
Symbol Table Structure

responsible for keeping track of symbols like variables and function names
this way we can determine the values of these identifiers by looking them up

List* symbols: the array list that holds each symbol
int current_offset: stores the integer representing the byte location of the last place a symbol was stored in memory
*/

#include "array_list.h"

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

//symbol definition
typedef struct symbol_struct {
    char* name;
    const char* type;
    int memory_offset;
} Symbol;

//symbol table
typedef struct symbol_table_struct {
    List* symbols;
    int current_offset;
} SymbolTable;

//create the symbol table
SymbolTable* init_symbol_table();
//add a symbol to table
void add_to_table(SymbolTable* table, char* name, const char* type);
//find a symbol already in table
Symbol* look_up_symbol(SymbolTable* table, char* name);
//free memory of a symbol
int free_symbol(Symbol* symbol);
//wrapper for freeing a symbol
void free_symbol_wrapper(void* symbol);
//free allocated table memory
int free_symbol_table(SymbolTable* table);

#endif