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
    //allocate for the symbol, using calloc for default values
    Symbol* symbol = calloc(1, sizeof(Symbol));
    //make a string copy for the name
    symbol->name = strdup(name);
    //set the kind
    symbol->kind = kind;

    //initalize param list if it is a function symbol
    if(kind == SYMBOL_FUNCTION) {
        symbol->data.func_sym.parameters = init_list(3);
    }

    return symbol;
}

/*
 * VariableStorage enum as a string. 
 */

char* storage_type_as_str(VariableSymbol symbol) {
    switch(symbol.storage) {
        case STORAGE_GLOBAL: return "STORAGE_GLOBAL";
        case STORAGE_LOCAL: return "STORAGE_LOCAL";
        case STORAGE_PARAMETER: return "STORAGE_PARAMETER";
        default: return "STORAGE_UNKNOWN";
    }
}

/*
 * SymbolKind enum as a string. 
 */

char* symbol_kind_as_str(Symbol* symbol) {
    switch(symbol->kind) {
        case SYMBOL_VARIABLE: return "SYMBOL_VARIABLE";
        case SYMBOL_FUNCTION: return "SYMBOL_FUNCTION";
        default: return "SYMBOL_UNKNOWN";
    }
}

/*
 * ToString for any symbol type.
 */

char* symbol_to_str(void* symbol) {
    Symbol* the_symbol = (Symbol*) symbol;
    //the string to return 
    char* symbol_str;

    if(the_symbol->kind == SYMBOL_VARIABLE) {
        //figure out length needed and allocate space for it
        int length = snprintf(NULL, 0, 
            "NAME: %s, KIND: %s, DATA TYPE: %s, STORAGE: %s, OFFSET: %d",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.var_sym.type), storage_type_as_str(the_symbol->data.var_sym), the_symbol->data.var_sym.offset);

        symbol_str = malloc(length + 1);

        //write to string
        snprintf(symbol_str, length + 1, 
            "NAME: %s, KIND: %s, DATA TYPE: %s, STORAGE: %s, OFFSET: %d",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.var_sym.type), storage_type_as_str(the_symbol->data.var_sym), the_symbol->data.var_sym.offset);
    }
    else if(the_symbol->kind == SYMBOL_FUNCTION) {
         int length = snprintf(NULL, 0, 
        "NAME: %s, KIND: %s, RETURN TYPE: %s",  
        the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.func_sym.return_type));

        symbol_str = malloc(length + 1);

        snprintf(symbol_str, length + 1, 
            "NAME: %s, KIND: %s, RETURN TYPE: %s",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.func_sym.return_type));
    }
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
