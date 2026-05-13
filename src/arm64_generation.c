/**
 * @file arm64_generation.c
 * @brief ast is traversed and assembly code is generated.
 */

#include "include/arm64_generation.h"


/**
 * @brief Turns in ast node to assembly code.
 */

void node_to_asm(FILE* file, ASTNode* node) {

}

/*
 * Writes the assembly to a file.
 */

void generate_assembly(FILE* file, Program* program) {
    //begin by extracting the root of the tree
    ASTNode* root = program->root;

    //setup the register manager
}
