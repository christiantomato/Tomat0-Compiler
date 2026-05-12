/**
 * @file hash_table.h
 * @brief Hash table for storing symbols effectively.
 * 
 * This hash table definition uses a polynomial hash function on a string key (symbol name) to 
 * generate the position. Collisions are resolved with separate chaining. 
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "symbol.h"
#include <stdio.h>

/**
 * @brief Defines a hash node for symbols. 
 * 
 * Implementing the singly linked lists for each hash position using pointers. 
 */

typedef struct hash_node_struct {
    Symbol* symbol; /**< The symbol stored at this node. */
    struct hash_node_struct* next; /** Pointer to the next node in this bucket. */
} HashNode;

/**
 * @struct HashTable
 * @brief Defines a hash table for our symbols. 
 */

typedef struct {
    HashNode** buckets; /**< Pointer to the list of "buckets", each which stores a linked list of nodes. */
    int size; /**< Size of the table. */
} HashTable;

/**
 * @brief Creates and initializes a new hash table. 
 * 
 * @param size The size of the table. 
 * @return Pointer to the newly created hash table. 
 */

HashTable* init_hash_table(int size);
/**
 * @brief Puts a symbol into the hash table. 
 * 
 * @param table Pointer to the hash table. 
 * @param symbol Pointer to the symbol. 
 */

void hash_put(HashTable* table, Symbol* symbol);

/**
 * @brief Gets a symbol from the hash table. 
 * 
 * @param table Pointer to the hash table. 
 * @param symbol_name Name of the symbol (acts as the hash key). 
 * @return Pointer to the symbol. 
 */

Symbol* hash_get(HashTable* table, const char* symbol_name);

/**
 * @brief Removes a symbol from the hash table. 
 * 
 * @param table Pointer to the table. 
 * @param symbol_name Name of the symbol (acts as the hash key). 
 */

void hash_remove(HashTable* table, const char* symbol_name);

/**
 * @brief ToString for a hash table. 
 * 
 * @param table Pointer to the table. 
 * @return String representation.
 */

char* hash_to_str(void* table);

/**
 * @brief Frees allocated memory by the hash table. 
 * 
 * Does not free the memory allocated by the symbols. That is managed by the scope. 
 * 
 * @param table Pointer to the table. 
 */

void free_hash_table(HashTable* table);

#endif
