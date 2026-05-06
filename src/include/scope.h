/**
 * @file scope.h
 * @brief Defines a scope. 
 * 
 * A scope is responsible for managing its own symbols. It will have a link to its parent scope
 * if it needs to look upward. Symbols are stored using a hash table to maximize efficiency, since 
 * look up time is O(1) average. 
 */

#ifndef SCOPE_H
#define SCOPE_H

#include "hash_table.h"
#include "symbol.h"

/**
 * @brief Struct definition of our scope. 
 */

typedef struct scope_struct {
    struct scope_struct* parent; /**< The parent scope. */
    HashTable* symbol_table; /**< The symbol table for the scope. */
    int current_offset; /**< The frame pointer offset for variables inside the scope. */
} Scope;

/**
 * @brief Creates and initalizes a new scope. 
 * 
 * @param parent Pointer to the parent scope. 
 * @return Pointer to the newly created scope. 
 */

Scope* init_scope(Scope* parent);

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

#endif
