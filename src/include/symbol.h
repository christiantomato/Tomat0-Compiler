/**
 * @file symbol.h
 * @brief Defines a symbol. 
 */

#ifndef SYMBOL_H
#define SYMBOL_H

/**
 * @enum SymbolType
 * @brief Enum for the different types of symbols we can encounter. 
 * 
 * These include: 
 * - variable names
 * - function names
 * - parameter names
 */

typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_PARAM
 } SymbolKind;

/**
 * @struct Symbol
 * @brief Defines the data needed to represent a symbol. 
 * 
 * This data is scope relative. 
 */

typedef struct {
    char* name; /**< The symbol name. */
    SymbolKind kind; /**< The kind of symbol. */
    int offset; /**< Offset relative to the frame pointer in its current stack frame. */

    //future? 
    //int size
    //DataType data_type;
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
