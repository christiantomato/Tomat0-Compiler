/**
 * @file types.c 
 * @brief Really just implements a string for the enum. 
 */

 #include "include/types.h"

/*
 * Returns enum as a string. 
 */

char* data_type_as_str(DataType type) {
    switch(type) {
        case TYPE_INT: return "TYPE_INT";
        case TYPE_STRING: return "TYPE_STRING";
        case TYPE_BOOL: return "TYPE_BOOL";
        default: return "TYPE_UNKNOWN";
    }
}
