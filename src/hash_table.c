/**
 * @file hash_table.c
 * @brief Implements the definition for our symbol hash table. 
 */

#include "include/hash_table.h"
#include <stdlib.h>
#include <string.h>

#define HASH_PRIME 37

/*
 * Creates and initializes a new hash table. 
 */

HashTable* init_hash_table(int size) {
    //create and allocate for the table
    HashTable* table = malloc(sizeof(HashTable));
    //allocate the array of pointers, using calloc so they aren't garbage values initally
    table->buckets = calloc(size, sizeof(HashNode*));
    //set the size
    table->size = size;
    return table;
}

/** 
 * @brief Generates the hash position using a polynomial hash key. 
 * 
 * Computes the polynomial using Horner's rule to prevent overflow. 
 * 
 * @param key The string key (will be the symbol name).
 * @return Bucket position. 
 */

static int hash_function(HashTable* table, const char* key) {
    //start with the first character in the key
    int hash_value = key[0];

    //loop through and compute the polynomial using Horner's rule
    for(int i = 1; key[i] != '\0'; i++) {
        hash_value = (hash_value * HASH_PRIME + (int) key[i]) % table->size;
    }

    return hash_value;
}

/*
 * Nodes are inserted to the head of the linked list. 
 */

void hash_put(HashTable* table, Symbol* symbol) {
    //get the index of where it should go 
    int index = hash_function(table, symbol->name);
    //get the head node and use it to traverse
    HashNode* curr = table->buckets[index];

    //traverse through and compare for identical symbol names
    while(curr != NULL) {
        //check if symbol names are the same (not allowed)
        if(strcmp(curr->symbol->name, symbol->name) == 0) {
            printf("Exception: duplicate symbol names.\n");
            return;
        }
        //move through the linked list
        curr = curr->next;
    }

    //create the new node
    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->symbol = symbol;
    //set next to old head
    new_node->next = table->buckets[index];
    //set the new node as the head
    table->buckets[index] = new_node;
}

/*
 * Gets a symbol from the table. 
 */

Symbol* hash_get(HashTable* table, const char* symbol_name) {
    //get the index of where it should be 
    int index = hash_function(table, symbol_name);
    //get the head node at that index
    HashNode* curr = table->buckets[index];

    //traverse to try and find the symbol
    while(curr != NULL) {
        //check if symbol names are the same
        if(strcmp(curr->symbol->name, symbol_name) == 0) {
            //found
            return curr->symbol;
        }
        //move through the linked list
        curr = curr->next;
    }

    //didn't find
    return NULL;
}

/*
 * Removes a symbol from the table.  
 */

void hash_remove(HashTable* table, const char* symbol_name) {
    //get the position of where it should be
    int index = hash_function(table, symbol_name);
    //get the head node at that index
    HashNode* curr = table->buckets[index];
    //introduce a previous node
    HashNode* prev = NULL;

    //traverse to find the one to remove
    while(curr != NULL) {
        //find the symbol
        if(strcmp(curr->symbol->name, symbol_name) == 0) {
            //removing head
            if(prev == NULL) {
                table->buckets[index] = curr->next;
            }
            //removing normal
            else {
                //skip over current
                prev->next = curr->next;
            }   
            //free it and return
            free(curr);
            return;
        }
        //move through the linked list
        prev = curr;
        curr = curr->next;
    }
    //wasnt found
    printf("Exception: node not found.\n");
}

/*
 * Prints the table representation to a file. 
 * Loops through each bucket and linked list. 
 */

void hash_print(FILE* file, HashTable* table) {
    //loop through each bucket
    for(int i = 0; i < table->size; i++) {
        //get the head
        HashNode* curr = table->buckets[i];

        //go through the linked list
        while(curr != NULL) {
            fprintf(file, "%s\n", symbol_to_str(curr->symbol));
            curr = curr->next;
        }
    }
}

/*
 * Frees allocated memory by the hash table.  
 */

void free_hash_table(HashTable* table) {
    //loop through each bucket
    for(int i = 0; i < table->size; i++) {
        //get the head
        HashNode* curr = table->buckets[i];

        //go through the linked list
        while(curr != NULL) {
            HashNode* temp = curr;
            //save pointer to the next
            curr = curr->next;
            //free node
            free(temp);
        }
    }
    
    //free the buckets
    free(table->buckets);
    //free the table itself
    free(table);
}
