/**
 * @file types.h
 * @brief This file just defines an enum for supported variables types. 
 */

#ifndef TYPES_H
#define TYPES_H

/**
 * @enum DataType
 * @brief Describes different types of data types supported in Tomat0. 
 */

 typedef enum {
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_FLOAT
 } DataType;

 /**
  * @brief Returns a string for the type enum. 
  * 
  * @param type The type enum. 
  * @return String representation. 
  */

char* data_type_as_str(DataType type);

#endif
