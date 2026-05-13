/**
 * @file symbol.h
 * @brief Defines a symbol. 
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include "types.h"
#include "array_list.h"
#include <stdbool.h>

/**
 * @enum SymbolType
 * @brief Enum for the different types of symbols we can encounter. 
 * 
 * These include: 
 * - variables
 * - functions
 */

typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
 } SymbolKind;

/**
 * @enum VariableStorage
 * @brief Enum for the different types of variable storage.
 */

typedef enum {
    STORAGE_GLOBAL,
    STORAGE_LOCAL,
    STORAGE_PARAMETER
} VariableStorage;

/**
 * @struct VariableSymbol
 * @brief Encodes necessary data needed for a variable.
 */

typedef struct {
    DataType type; /**< The data type. */
    VariableStorage storage; /**< The method of storage for the variable. */
    int offset; /**< The stack frame relative offset. */
} VariableSymbol;

/**
 * @struct FunctionSymbol
 * @brief Encpdes necessary data needed for a function. 
 */

typedef struct {
    DataType return_type; /**< The return type. */
    List* parameters; /**< The list of parameters. */
} FunctionSymbol;

/**
 * @union SymbolData
 * @brief A union so we can store specific data for each type.
 */

typedef union {
    VariableSymbol var_sym;
    FunctionSymbol func_sym;
} SymbolData;

/**
 * @struct Symbol
 * @brief Defines all symbol types. 
 */

typedef struct {
    char* name; /**< The symbol name. */
    SymbolKind kind; /**< The kind of symbol. */
    SymbolData data; /** The data for the symbol. */
} Symbol;

/**
 * @brief Creates and initializes a new symbol. 
 * 
 * @param name The name of the symbol. 
 * @param kind The kind of symbol. 
 * @return Pointer to the created symbol. 
 */

Symbol* init_symbol(char* name, SymbolKind kind); 

/**
 * @brief Returns the enum for symbol kind as a string. 
 * 
 * @param symbol Pointer to the symbol. 
 * @return String representation of the enum. 
 */

char* symbol_kind_as_str(Symbol* symbol);

/**
 * @brief A toString for a symbol. 
 * 
 * @param symbol Generic pointer to the symbol.
 * @return String representation of the symbol. 
 */

char* symbol_to_str(void* symbol);

/**
 * @brief Frees memory allocated by a symbol. 
 * 
 * @param symbol Pointer to the symbol. 
 */

void free_symbol(Symbol* symbol);

#endif
