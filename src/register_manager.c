/**
 * @file register_manager.c
 * @brief Implements definition of our register manager.
 */

#include "include/register_manager.h"
#include <stdlib.h>

/*
 * Initalize the manager.
 */

RegisterManager* init_register_manager() {
    //allocate manager
    RegisterManager* manager = malloc(sizeof(RegisterManager));

    //initally set all registers as free

    //param register
    for(int i = 0; i < 8; i++) {
        manager->param[i] = true;
    }
    //general
    for(int i = 0; i < 7; i++) {
        manager->general[i] = true;
    }
    //callee
    for(int i = 0; i < 10; i++) {
        manager->callee[i] = true;
    }

    return manager;
}

/*
 * Allocate a parameter register.
 */

int allocate_param_register(RegisterManager* manager) {
    //check which param register is free
    for(int i = 0; i < 8; i++) {
        if(manager->param[i]) {
            //set as allocated and return
            manager->param[i] = false;
            return i;
        }
    }
    //otherwise no registers free
    return -1;
}

/*
 * Allocate a parameter register.
 */

int allocate_general_register(RegisterManager* manager) {
    //check which general register is free
    for(int i = 0; i < 7; i++) {
        if(manager->general[i]) {
            //set as allocated and return
            manager->general[i] = false;
            //make sure to return actual num
            return i + 9;
        }
    }
    //otherwise no registers free
    return -1;
}

/*
 * Allocate a parameter register.
 */

int allocate_callee_register(RegisterManager* manager) {
    //check which callee register is free
    for(int i = 0; i < 10; i++) {
        if(manager->callee[i]) {
            //set as allocated and return
            manager->callee[i] = false;
            //make sure to return actual number
            return i + 19;
        }
    }
    //otherwise no registers free
    return -1;
}

/*
 * Free the specified register.
 */

void free_register(RegisterManager* manager, int register_num) {
    if(register_num <= 7) {
        //param register
        manager->param[register_num] = true;
    }
    else if(register_num >= 9 && register_num <= 15) {
        //general register
        manager->general[register_num - 9] = true;
    }
    else if(register_num >= 19 && register_num <= 28) {
        //callee register
        manager->callee[register_num - 19] = true;
    }
}

/*
 * Free allocated memory. 
 */

void free_reg_manager(RegisterManager* manager) {
    //free manager itself.
    free(manager);
}
