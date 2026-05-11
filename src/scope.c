/**
 * @file scope.c
 * @brief Implements my scope definition. 
 */

#include "include/scope.h"
#include <stdlib.h>

#define SYMBOL_TABLE_SIZE 16

/*
 * Initalizses a new scope. 
 */

Scope* init_scope(Scope* parent) {
    //create and allocate for the scope
    Scope* scope = malloc(sizeof(Scope));
    //set the parent
    scope->parent = parent;
    //initalize a symbol_table
    scope->symbol_table = init_hash_table(SYMBOL_TABLE_SIZE); 
    //set initial offset to 0
    scope->current_offset = 0;
    return scope;
}

/*
 * Looks for the symbol in each scope, all the way until the global scope.
 */

Symbol* lookup_symbol(Scope* scope, const char* name) {
    //traverse through each scope until global
    Scope* current = scope;
    while(current != NULL) {
        //get with the hash get function
        Symbol* symbol = hash_get(current->symbol_table, name);

        //return if found, otherwise go up
        if(symbol != NULL) return symbol;
        else current = current->parent;
    }

    //not found
    return NULL;
}

/*
 * Looks for the symbol just in this scope (to avoid duplicates).
 */

Symbol* lookup_symbol_in_scope(Scope* scope, const char* name) {
    //return result from hash get on the symbol table
    return hash_get(scope->symbol_table, name);
}

/*
 * Adds a symbol to the symbol table of the specified scope. 
 */

void add_symbol(Scope* scope, Symbol* symbol) {
    hash_put(scope->symbol_table, symbol);
}

/*
 * Creates a new scope and initializes with parent.
 * Not much, but good for clean readability and flow. 
 */

Scope* enter_scope(Scope* current_scope) {
    Scope* nested_scope = init_scope(current_scope);
    return nested_scope;
}

/*
 * Exits the current scope and returns the parent scope. 
 * Again, just for clean flow. 
 */

Scope* exit_scope(Scope* current_scope) {
    Scope* outer_scope = current_scope->parent;
    return outer_scope;
}

/*
 * Frees allocated memory by scope.
 */

void free_scope(Scope* scope) {
    //free the symbol table
    free(scope->symbol_table);
    //free scope itself
    free(scope);
}

/*
 * To be able to pass to array list. 
 */

void free_scope_wrapper(void* scope) {
    free_scope((Scope*) scope);
}
