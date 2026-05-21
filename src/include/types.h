/**
 * @file types.h
 * @brief This file just defines an enum for supported variables types. 
 */

#ifndef TYPES_H
#define TYPES_H

/**
 * @enum DataType
 * @brief Describes different types of data types supported in Tomat0. 
 * 
 * Note: Strings will be immutable. 
 */

 typedef enum {
    TYPE_INT, /**< Integers. */
    TYPE_STRING, /**< Strings. */
    TYPE_BOOL, /**< Booleans (1 for true, 0 for false). */
    TYPE_VOID /**< Use for unresolved types or for functions with no return. */
 } DataType;

 /**
  * @brief Returns a string for the type enum. 
  * 
  * @param type The type enum. 
  * @return String representation. 
  */

char* data_type_as_str(DataType type);

#endif
