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
    int current_offset; /**< The next free stack slot relative to the frame pointer of this scope. */
} Scope;

/**
 * @brief Creates and initalizes a new scope. 
 * 
 * @param parent Pointer to the parent scope. 
 * @return Pointer to the newly created scope. 
 */

Scope* init_scope(Scope* parent);

/**
 * @brief Looks up the desired symbol in all scopes. 
 * 
 * Nested scopes can use symbols from higher level scopes, so when we look up a symbol, 
 * we must traverse all the way up to the global scope to look for it. 
 * 
 * @param scope Pointer to the current scope. 
 * @param name Name of symbol we are looking for. 
 * @return Pointer to the symbol. 
 */

Symbol* lookup_symbol(Scope* scope, const char* name);

/**
 * @brief Looks up the desired symbol in only this scope. 
 * 
 * Each individual scope cannot have duplicate variable names, so this function is used 
 * to make sure the symbol name doesn't already exist in this scope but its okay if it exists in higher scopes! (shadowing).
 * 
 * @param scope Pointer to the current scope. 
 * @param name Name of symbol we are looking for.
 * @return Pointer to the symbol.
 */

Symbol* lookup_symbol_in_scope(Scope* scope, const char* name);

/**
 * @brief Adds a symbol to the symbol table of the scope. 
 * 
 * @param scope Pointer to the scope. 
 * @param symbol Pointer to the symbol.
 */

void add_symbol(Scope* scope, Symbol* symbol);

/**
 * @brief Enter a scope.
 * 
 * @param current_scope Pointer to the current scope. 
 * @return Pointer to the nested scope. 
 */

Scope* enter_scope(Scope* current_scope);

/**
 * @brief Exits a scope. 
 * 
 * @param current_scope Pointer to the current scope. 
 * @return Pointer to the parent scope. 
 */

Scope* exit_scope(Scope* current_scope);

/**
 * @brief Frees allocated memory by the scope. 
 * 
 * @param scope Pointer to the scope. 
 */

void free_scope(Scope* scope);

#endif
