/**
 * @file ll_stack.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Stack data structure for long integers.
 * @version 0.1
 * @date 2022-09-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LL_STACK_H
#define LL_STACK_H

#include <cstdlib>
#include <cstdint>
#include "stackreports.h"

typedef long long ll_stack_content_t;
typedef void* const LLStack;

/**
 * @brief Construct stack and return its encrypted address.
 * 
 * @param size number of elements in the stack
 * @param err_code variable to use as errno
 * @return LLStack 
 */
LLStack ll_stack_ctor(size_t size, int* const err_code = NULL);

/**
 * @brief Destroy the stack.
 * 
 * @param stack encrypted pointer to the stack
 */
void ll_stack_dtor(LLStack stack);

/**
 * @brief Push one element to the stack.
 * 
 * @param stack encrypted pointer to stack
 * @param value value to push
 * @param err_code variable to use as errno
 */
void ll_stack_push(LLStack stack, const ll_stack_content_t value, int* const err_code = NULL);

/**
 * @brief Get the last element of the stack or POISON if it is empty.
 * 
 * @param stack encrypted pointer to the stack
 * @param err_code variable to use as errno
 * @return ll_stack_content_t 
 */
ll_stack_content_t ll_stack_pull(LLStack stack, int* const err_code = NULL);

/**
 * @brief Erase the last element of the stack and do nothing if it is empty.
 * 
 * @param stack encrypted pointer to the stack
 * @param err_code variable to use as errno
 */
void ll_stack_pop(LLStack stack, int* const err_code = NULL);

/**
 * @brief Get stack status.
 * 
 * @param stack encrypted pointer to the stack
 * @return stack_report_t 
 */
stack_report_t ll_stack_status(LLStack stack);

/**
 * @brief Dump the stack into logs.
 * 
 * @param stack encrypted pointer to the stack
 * @param importance importanc eof the message
 */
#define ll_stack_dump(stack, importance) _ll_stack_dump(stack, importance, __PRETTY_FUNCTION__, __LINE__, __FILE__)
void _ll_stack_dump(LLStack stack, int importance, const char* function, const size_t line, const char* file);

/**
 * @brief Get stack size.
 * 
 * @param stack encrypted pointer to the stack
 * @param err_code variable to use as errno
 * @return uintptr_t 
 */
uintptr_t ll_stack_size(LLStack stack, int* const err_code = NULL);

/**
 * @brief Get stack allocated size.
 * 
 * @param stack encrypted pointer to the stack
 * @param err_code variable to use as errno
 * @return uintptr_t 
 */
uintptr_t ll_stack_capacity(LLStack stack, int* const err_code = NULL);

#endif