/*
Register Manager Structure

manages the registers in use during assembly code generation

bool registers[16]: represents the state of the registers x0-x15 (false if in use, true if free)
*/

#ifndef REGISTER_MANAGER_H
#define REGISTER_MANAGER_H

#include <stdbool.h>

typedef struct register_manager_struct {
    bool registers[16];
} RegisterManager;

//initializes the manager
RegisterManager* init_register_manager();
//returns the number of the register that was allocated
int allocate_register(RegisterManager* manager);
//returns a register from x8-x15 so old register isn't clobbered by function arguments
int allocate_safe_register(RegisterManager* manager, int old_register);
//frees the specified register
void free_register(RegisterManager* manager, int register_num);

#endif