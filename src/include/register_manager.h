/**
 * @file register_manager.h
 * @brief For managing the state of the registers.
 * 
 * List of the register uses: 
 * x0: return register
 * x0-x7: passing parameters
 * x9-x15: general purpose
 * x16: system call numbers
 * x19-x28: callee-saved registers (we are going to abuse these for general)
 * x29: frame pointer
 * x30: link register
 * sp: stack pointer
 */

#ifndef REGISTER_MANAGER_H
#define REGISTER_MANAGER_H

#include <stdbool.h>

/**
 * @struct RegisterManager
 * @brief Describes the state of each register that we will be using.
 */

typedef struct {
    bool param[8]; /**< The parameter register. */
    bool general[7]; /**< The general purpose registers. */
    bool callee[10]; /**< Callee-saved registers. */
} RegisterManager;

/**
 * @brief Initializes the manager.
 * 
 * @return Pointer to the register manager.
 */

RegisterManager* init_register_manager();

/**
 * @brief Allocates a parameter register.
 * 
 * @param manager Pointer to the register manager.
 * @return Register number of the next available register.
 */

int allocate_param_register(RegisterManager* manager);

/**
 * @brief Allocates a general register.
 * 
 * @param manager Pointer to the register manager.
 * @return Register number of the next available register.
 */

int allocate_general_register(RegisterManager* manager);

/**
 * @brief Allocates a callee register.
 * 
 * @param manager Pointer to the register manager.
 * @return Register number of the next available register.
 */

int allocate_callee_register(RegisterManager* manager);

/**
 * @brief Frees a register for use.
 * 
 * @param manager Pointer to the register manager.
 * @param register_num Register we want to free up.
 */

void free_register(RegisterManager* manager, int register_num);

/**
 * @brief Frees allocated memory by the register manager.
 * 
 * @param manager Pointer to the register manager.
 */

void free_reg_manager(RegisterManager* manager);

#endif