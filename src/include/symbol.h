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
 */

typedef enum {
    SYMBOL_VARIABLE, /**< Integers and booleans. */
    SYMBOL_FUNCTION, /**< Functions. */
    SYMBOL_STRING, /**< Strings, which are immutable. */
    SYMBOL_PARAMETER /**< Function parameters. */
 } SymbolKind;

/**
 * @struct VariableSymbol
 * @brief Encodes necessary data needed for a variable.
 */

typedef struct {
    DataType type; /**< The data type. */
    int offset; /**< The stack frame relative offset. */
} VariableSymbol;

/**
 * @struct FunctionSymbol
 * @brief Encodes necessary data needed for a function. 
 */

typedef struct {
    DataType return_type; /**< The return type. */
    List* parameters; /**< The list of parameters. */
} FunctionSymbol;

/**
 * @struct StringSymbol
 * @brief Encodes necessary data needed for a string.
 */

typedef struct {
    char* label; /**< The .data label. */
} StringSymbol;

/**
 * @struct ParameterSymbol
 * @brief Encodes necessary data needed for a parameter.
 */

typedef struct {
    DataType type; /**< The parameter data type. */
    int reg; /**< The register it will be passed through in (x0-x7) */
} ParameterSymbol;

/**
 * @union SymbolData
 * @brief A union so we can store specific data for each type.
 */

typedef union {
    VariableSymbol var_data;
    FunctionSymbol func_data;
    StringSymbol str_data;
    ParameterSymbol param_data;
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

/**
 * @brief Wrapper for free function.
 * 
 * @param symbol Pointer to the symbol. 
 */

void free_symbol_wrapper(void* symbol);

#endif
