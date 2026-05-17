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
        symbol->data.func_data.parameters = init_list(3);
    }

    return symbol;
}

/*
 * SymbolKind enum as a string. 
 */

char* symbol_kind_as_str(Symbol* symbol) {
    switch(symbol->kind) {
        case SYMBOL_VARIABLE: return "SYMBOL_VARIABLE";
        case SYMBOL_FUNCTION: return "SYMBOL_FUNCTION";
        case SYMBOL_STRING: return "SYMBOL_STRING";
        case SYMBOL_PARAMETER: return "SYMBOL_PARAMETER";
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
            "SYMBOL NAME: %s, KIND: %s, DATA TYPE: %s, OFFSET: %d",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.var_data.type), the_symbol->data.var_data.offset);

        symbol_str = malloc(length + 1);

        //write to string
        snprintf(symbol_str, length + 1, 
            "SYMBOL NAME: %s, KIND: %s, DATA TYPE: %s, OFFSET: %d",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.var_data.type), the_symbol->data.var_data.offset);
    }
    else if(the_symbol->kind == SYMBOL_FUNCTION) {
        int length = snprintf(NULL, 0, 
            "SYMBOL NAME: %s, KIND: %s, RETURN TYPE: %s",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.func_data.return_type));

        symbol_str = malloc(length + 1);

        snprintf(symbol_str, length + 1, 
            "SYMBOL NAME: %s, KIND: %s, RETURN TYPE: %s",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.func_data.return_type));
    }
    else if(the_symbol->kind == SYMBOL_STRING) {
        //figure out length needed and allocate space for it
        int length = snprintf(NULL, 0, 
            "SYMBOL NAME: %s, KIND: %s, LABEL: %s",  
            the_symbol->name, symbol_kind_as_str(the_symbol), the_symbol->data.str_data.label);

        symbol_str = malloc(length + 1);

        //write to string
        snprintf(symbol_str, length + 1, 
            "SYMBOL NAME: %s, KIND: %s, LABEL: %s",  
            the_symbol->name, symbol_kind_as_str(the_symbol), the_symbol->data.str_data.label);
    }
    else if(the_symbol->kind == SYMBOL_PARAMETER) {
        //figure out length needed and allocate space for it
        int length = snprintf(NULL, 0, 
            "SYMBOL NAME: %s, KIND: %s, TYPE: %s, REGISTER: %d, OFFSET, %d",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.param_data.type), the_symbol->data.param_data.reg, the_symbol->data.param_data.offset);

        symbol_str = malloc(length + 1);

        //write to string
        snprintf(symbol_str, length + 1, 
            "SYMBOL NAME: %s, KIND: %s, TYPE: %s, REGISTER: %d, OFFSET, %d",  
            the_symbol->name, symbol_kind_as_str(the_symbol), data_type_as_str(the_symbol->data.param_data.type), the_symbol->data.param_data.reg, the_symbol->data.param_data.offset);
    }

    return symbol_str;
}

/*
 * Frees allocated memory by symbol. 
 */

void free_symbol(Symbol* symbol) {
    //free name
    free(symbol->name);

    switch(symbol->kind) {
        case SYMBOL_VARIABLE:
            //nothin
            break;
        case SYMBOL_FUNCTION:
            free_list(symbol->data.func_data.parameters, free_symbol_wrapper);
            break;
        case SYMBOL_STRING:
            //free label name
            free(symbol->data.str_data.label);
            break;
        case SYMBOL_PARAMETER:
            //nothing
            break;
        default: return;
    }

    //free symbol itself
    free(symbol);
}

/*
 * Wrapper for free symbol.
 */

void free_symbol_wrapper(void* symbol) {
    Symbol* the_symbol = (Symbol*) symbol;
    free_symbol(the_symbol);
}
