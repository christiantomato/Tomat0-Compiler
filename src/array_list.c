#include "include/array_list.h"
#include <stdlib.h>

/*
Initialize List Function

creates a list for any data type
the array holds void pointers, allowing us to use it for any data type

unsigned int init_capacity: sets the initial capacity of the list

return: pointer to the list
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
List Add Function

appends an item at the end of the list

List* list: the list the item is being added to
void* data: the piece of data being added
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
List Remove Function

removes a data item from the list, at any index
option to provide a free function for complex data types (such as tokens or ast nodes)

List* list: the list we are removing from
unsigned int index: the location in the list we are freeing
void (*free_func) (void*): a pointer to a function that takes in a void pointer, and returns void (for freeing a complex data type)
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

/*
Expand List Capacity Function

helper method to increase the size of the array in the array list
expands by double the current size

List* list: the list which is being expanded
*/

void expand_capacity(List* list) {
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

/*
Is List Empty Function

checks if the list is empty or not

List* list: the list we are checking

return: true if empty, false otherwise
*/

bool is_empty(List* list) {
    return list->num_items == 0;
}

/*
Is List At Max Capacity Function

checks if the list is full or not

List* list: the list we are checking

return: true if full, false otherwise
*/
bool is_max_capacity(List* list) {
    return list->num_items == list->current_capacity;
}

/*
Free Primitive List

frees the memory of the list container, with the option to free data items completely (for a primitive type)

List* list: the list being freed
bool free_deep: true to free data items completely, false to just free the container

return: 0 for success, 1 otherwise
*/

int free_primitive_list(List* list, bool free_deep) {
    //ensure the list isn't garbage
    if(list == NULL) {
        return 1;
    }
    if(free_deep) {
        //free the memory each pointer in the array is pointing to
        for(int i = 0; i < list->num_items; i++) {
            free(list->array[i]);
        }
    }
    //free the array 
    free(list->array);
    //free the list structure itself
    free(list);
    return 0;
}

/*
Free Complex List

frees the memory of the list container, with the option to completely free the data items (for a complex type)

List* list: the list being freed
void (*free_func)(void*): the function to free the complex data type

return: 0 for success, 1 otherwise
*/

int free_complex_list(List* list, void (*free_func)(void*)) {
    //ensure the list isn't garbage
    if(list == NULL) {
        return 1;
    }
    //if provided
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
Print List Function

uses a function pointer to print out the list of any generic type to a file

FILE* file: the file we are printing to 
List* list: the list which we are printing
char* (*to_string_func)(void*): the function pointer which performs the to string on the data type
*/

void print_list(FILE* file, List* list, char* (*to_string_func)(void*)) {
    for(int i = 0; i < list->num_items; i++) {
        fprintf(file, "%s\n", to_string_func(list->array[i]));
    }
}
