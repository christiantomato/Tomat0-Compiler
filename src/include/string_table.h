/*
String Table Structure

stores all string literals defined

*/

#include "include/array_list.h"

#ifndef STRING_TABLE_H
#define STRING_TABLE_H

typedef struct string_table_struct {
    List* strings;
} StringTable;

//create the string table
StringTable* init_string_table();
//add a string to the table
void add_string_to_table();


#endif