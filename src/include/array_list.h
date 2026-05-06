/**
 * @file array_list.h
 * @brief Generic dynamic array list.
 *
 * Definition of my own array list that can store pointers to any data type.
 * Commonly used for storing tokens and AST nodes.
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @struct List
 * @brief Generic array list structure.
 */

typedef struct {
    unsigned int num_items; /**< Number of elements currently stored in the list. */
    unsigned int current_capacity; /**< Maximum number of elements the list can hold before resizing. */
    void** array; /**< Pointer to the underlying array which stores pointers to the data type of the list. */
} List;

/**
 * @brief Creates and initializes a new list.
 *
 * @param init_capacity Initial capacity of the list.
 * @return List* Pointer to the newly created list.
 */

List* init_list(unsigned int init_capacity);

/**
 * @brief Adds an element to the end of the list.
 *
 * @param list Pointer to the list.
 * @param data Pointer to the data to add.
 */

void list_add(List* list, void* data);

/**
 * @brief Removes an element at a specified index.
 *
 * @param list Pointer to the list.
 * @param index Index of the element to remove.
 * @param free_func Function used to free the element's memory.
 */

void list_remove(List* list, unsigned int index, void (*free_func)(void*));

/**
 * @brief Checks if the list is empty.
 *
 * @param list Pointer to the list.
 * @return true if the list is empty, false otherwise.
 */

bool is_empty(List* list);

/**
 * @brief Checks if the list has reached maximum capacity.
 *
 * @param list Pointer to the list.
 * @return true if full, false otherwise.
 */

bool is_max_capacity(List* list);

/**
 * @brief Frees a list and optionally its elements. 
 *
 * @param list Pointer to the list.
 * @param free_func Function used to free each element.
 */

void free_list(List* list, void (*free_func)(void*));

/**
 * @brief Prints the list to a file.
 *
 * @param file Output file stream.
 * @param list Pointer to the list.
 * @param to_string_func Function to convert elements to null-terminated strings. 
 */

void print_list(FILE* file, List* list, char* (*to_string_func)(void*));

#endif
