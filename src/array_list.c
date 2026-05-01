/**
 * @file array_list.c
 * @brief Generic dynamic array list implementation.
 *
 * Implements the defined functions for my array list. 
 */

#include "include/array_list.h"
#include <stdlib.h>

/*
 * Allocates memory for the list structure and its underlying array of void pointers,
 * allowing it to store any data type.
 */

List* init_list(unsigned int init_capacity) {
    //allocate memory for the list
    List* list = malloc(sizeof(List));
    list->num_items = 0;
    list->current_capacity = init_capacity;
    //initialize the amount of locations in memory available, size of void pointers since we do not know the data type
    list->array = malloc(init_capacity * sizeof(void*));
    return list;
}

/*
 * If the list is at maximum capacity, it is expanded before the element is appended.
 */

void list_add(List* list, void* data) {
    if(is_max_capacity(list)) {
        expand_capacity(list);
    }
    //add to end of list
    list->array[list->num_items] = data;
    //update count
    list->num_items++;
}

/*
 * Shifts all elements after index left to fill the gap. If a free function is provided,
 * it is called on the element before removal.
 */

void list_remove(List* list, unsigned int index, void (*free_func)(void*)) {
    //make sure the index is valid
    if(index >= list->num_items) {
        printf("cannot remove");
        return;
    }
    //if a free function was passed in
    if(free_func) {
        //free data item completely from memory
        free_func(list->array[index]);
    }
    //shift the list over
    for(int i = index + 1; i < list->num_items; i++) {
        list->array[i-1] = list->array[i];
    }
    //update item count
    list->num_items--;
}

 /**
 * @brief Expands the capacity of the list.
 *
 * Helper which allocates a new array at double the current capacity, 
 * copies all existing pointers into it, frees the old array, and updates the list's capacity.
 *
 * @param list Pointer to the list.
 */

static void expand_capacity(List* list) {
    //double the size
    void** new_array = malloc(list->current_capacity * 2 * sizeof(void*));
    //update the current capacity
    list->current_capacity = list->current_capacity * 2;
    //copy the pointers into the new array
    for(int i = 0; i < list->num_items; i++) {
        new_array[i] = list->array[i];
    }
    //free the old array pointers
    free(list->array);
    //set the new array
    list->array = new_array;
}

bool is_empty(List* list) {
    return list->num_items == 0;
}

bool is_max_capacity(List* list) {
    return list->num_items == list->current_capacity;
}

/*
 * If a free function is provided, it is called on each element before
 * the array and list structure are freed.
 */

int free_list(List* list, void (*free_func)(void*)) {
    //ensure the list isn't garbage
    if(list == NULL) {
        return 1;
    }

    //if provided free the data stored
    if(free_func) {
        //free deep
        for(int i = 0; i < list->num_items; i++) {
            //free pointer to data in the array
            free_func(list->array[i]);
        }
    }

    //free the array
    free(list->array);
    //free the list structure itself
    free(list);
    return 0;
}

/*
 * Iterates over all elements, calling to_string_func on each one and writing
 * the result to the specified file stream, each on its own line.
 */

void print_list(FILE* file, List* list, char* (*to_string_func)(void*)) {
    for(int i = 0; i < list->num_items; i++) {
        fprintf(file, "%s\n", to_string_func(list->array[i]));
    }
}
