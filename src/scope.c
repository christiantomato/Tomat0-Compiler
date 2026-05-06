/**
 * @file scope.c
 * @brief Implements my scope definition. 
 */

#include "include/scope.h"

/*
 * Initalizses a new scope. 
 */

Scope* init_scope(Scope* parent) {
    Scope* scope = malloc(sizeof(Scope));
}

/**
 * @brief Looks up the desired symbol in the scope. 
 * 
 * @param scope Pointer to the scope. 
 * @param name Name of symbol we are looking for. 
 * @return Pointer to the symbol. 
 */

Symbol* lookup_symbol(Scope* scope, const char* name);

/**
 * @brief Adds a symbol to the symbol table of the scope. 
 * 
 * @param scope Pointer to the scope. 
 * @param symbol Pointer to the symbol.
 */

void add_symbol(Scope* scope, Symbol* symbol);

/**
 * @brief Frees allocated memory by the scope. 
 * 
 * @param scope Pointer to the scope. 
 */

void free_scope(Scope* scope);