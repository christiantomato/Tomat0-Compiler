/**
 * @file symbol.c
 * @brief Implements symbol.h functions. 
 */

#include "include/symbol.h"
#include <stdlib.h>
#include <string.h>

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
 * Frees allocated memory by symbol. 
 */

void free_symbol(Symbol* symbol) {
    free(symbol->name);
    free(symbol);
}
