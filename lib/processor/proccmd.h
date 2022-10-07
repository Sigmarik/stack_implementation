/**
 * @file proccmd.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief List of processor commands
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PROC_CMD_H
#define PROC_CMD_H

enum CMD_LIST {
    CMD_END,        //* End program.
    CMD_PUSH,       //* Push element to the stack.
    CMD_POP,        //* Remove element from the stack.
    CMD_OUT,        //* Print element of the stack to the screen.
    CMD_ADD,        //* Add last two elements.
    CMD_SUB,        //* Subtract last two elements from each other.
    CMD_MUL,        //* Multiply last two elements.
    CMD_DIV,        //* Divide last two elements.
    CMD_DUP,        //* Push copy of last element to the stack.
    CMD_ABORT,      //* Abort program execution.
};

#endif