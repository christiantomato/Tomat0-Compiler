/**
 * @file symbol.c
 * @brief Implements functions for my symbol definition. 
 */

#include "include/symbol.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Creates and initalizes a symbol. 
 */

Symbol* init_symbol(char* name, SymbolKind kind) {
    //create a symbol
    Symbol* symbol = malloc(sizeof(Symbol));
    //make a string copy for the name
    symbol->name = strdup(name);
    //set the kind
    symbol->kind = kind;
    //set default offset (handled later by scope)
    symbol->offset = 0;

    return symbol;
}

/*
 * SymbolKind enum as a string. 
 */

char* symbol_kind_as_str(Symbol* symbol) {
    switch(symbol->kind) {
        case SYMBOL_VARIABLE: return "SYMBOL_VARIABLE";
        case SYMBOL_FUNCTION: return "SYMBOL_FUNCTION";
        case SYMBOL_PARAM: return "SYMBOL_PARAM";
        default: return "SYMBOL_UNKNOWN";
    }
}

char* symbol_to_str(void* symbol) {
    Symbol* the_symbol = (Symbol*) symbol;
    //the string to return 
    char* symbol_str;

    //figure out length needed and allocate space for it
    int length = snprintf(NULL, 0, 
        "SYMBOL NAME: %s, SYMBOL KIND: %s, FRAME POINTER OFFSET: %d", 
        the_symbol->name, symbol_kind_as_str(the_symbol), the_symbol->offset);
    
    symbol_str = malloc(length + 1);

    //build the string
    snprintf(symbol_str, length + 1, 
        "SYMBOL NAME: %s, SYMBOL KIND: %s, FRAME POINTER OFFSET: %d",
        the_symbol->name, symbol_kind_as_str(the_symbol), the_symbol->offset);
    return symbol_str;
}

/*
 * Frees allocated memory by symbol. 
 */

void free_symbol(Symbol* symbol) {
    //free name since we strdup'd it
    free(symbol->name);
    //free symbol itself
    free(symbol);
}
