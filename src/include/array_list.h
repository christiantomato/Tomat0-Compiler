/*
Generic Array List Structure

definition of my own array list, for any data type
used so I can store things like tokens and ast nodes in an organized and efficient manner

unsigned int num_items: the number of items in the list
unsigned int current_capacity: the current capacity of items that the list can hold
void** array: the pointer to the array, which is an array that holds pointers to data
*/

#include <stdbool.h>
#include <stdio.h>

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

typedef struct generic_array_list_struct {
    unsigned int num_items;
    unsigned int current_capacity;
    void** array;
} List;

//create a list
List* init_list(unsigned int init_capacity);
//add data to the end of list
void list_add(List* list, void* data);
//remove data at specified index
void list_remove(List* list, unsigned int index, void (*free_func)(void*));
//helper to expand the array size
void expand_capacity(List* list);
//checks if empty
bool is_empty(List* list);
//checks if full
bool is_max_capacity(List* list);
//free all data in list for simple data types 
int free_primitive_list(List* list, bool free_deep);
//free all memory in list storing complex data types
int free_complex_list(List* list, void (*free_func)(void*));
//print a list of any type, to a file
void print_list(FILE* file, List* list, char* (*to_string_func)(void*));

#endif
