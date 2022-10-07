/**
 * @file stackworks_impl.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Implementation of functions defined in _stackworks.h
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef STACKWORKS_IMPL_H
#define STACKWORKS_IMPL_H

#include <cstddef>
#include <cstdlib>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>
#include "_stackworks.h"

void stack_init(Stack* const stack, const size_t size, int* const err_code) {
    stack_report_t status = stack_status(stack);
    _LOG_FAIL_CHECK_(!(status & ~(STACK_NULL_CONTENT|STACK_HASH_FAILURE)), "error", ERROR_REPORTS, return, err_code, EINVAL);

    stack->buffer = _stack_alloc_space(size, err_code);
    _LOG_FAIL_CHECK_(stack->buffer, "error", ERROR_REPORTS, return, err_code, ENOMEM);
    
    stack->capacity = size;

    ON_HASH(stack->_hash = _stack_hash(stack));
}

void stack_destroy(Stack* const stack, int* const err_code) {
    _LOG_FAIL_CHECK_(!stack_status(stack), "error", ERROR_REPORTS, return, err_code, EINVAL);

    free(stack->buffer);
    stack->buffer = NULL;

    stack->size = 0;
    stack->capacity = 0;

    ON_HASH(stack->_hash = 0);
}

void stack_push(Stack* const stack, const stack_content_t value, int* const err_code) {
    _LOG_FAIL_CHECK_(!stack_status(stack), "error", ERROR_REPORTS, return, err_code, EINVAL);

    if (stack->capacity < stack->size + 1) {
        _stack_change_size(stack, stack->capacity * STACK_BUFFER_INCREASE, err_code);
    }

    _stack_content(stack)[stack->size] = value;

    ++stack->size;

    ON_HASH(stack->_hash = _stack_hash(stack));
}

void stack_pop(Stack* const stack, int* const err_code) {
    _LOG_FAIL_CHECK_(!stack_status(stack), "error", ERROR_REPORTS, return, err_code, EINVAL);
    _LOG_FAIL_CHECK_(stack->size, "error", ERROR_REPORTS, return, err_code, ENXIO);

    if ((stack->size - 1) * STACK_BUFFER_INCREASE * STACK_BUFFER_INCREASE < stack->capacity) {
        _stack_change_size(stack, stack->capacity / STACK_BUFFER_INCREASE, err_code);
    }

    _stack_content(stack)[stack->size - 1] = STACK_CONTENT_POISON;
    --stack->size;

    ON_HASH(stack->_hash = _stack_hash(stack));
}

stack_content_t stack_get(Stack* const stack, int* const err_code) {
    _LOG_FAIL_CHECK_(!stack_status(stack), "error", ERROR_REPORTS, return STACK_CONTENT_POISON, err_code, EINVAL);
    return _stack_content(stack)[stack->size - 1];
}

stack_report_t stack_status(const Stack* const stack) {
    stack_report_t status = 0;

    if (check_ptr(stack) == false) return STACK_NULL;

    ON_CANARY(if (stack->size > stack->capacity) status |= STACK_BIG_SIZE);

    if (check_ptr(stack->buffer) == false) status |= STACK_NULL_CONTENT;

    ON_CANARY({
        if (!stack_check_canary(stack->_canary_left))  status |= STACK_L_CANARY_FAIL;
        if (!stack_check_canary(stack->_canary_right)) status |= STACK_R_CANARY_FAIL;

        if (check_ptr(stack->buffer) && !stack_check_canary(stack->buffer))
            status |= STACK_BL_CANARY_FAIL;
        if (check_ptr(stack->buffer) && !stack_check_canary((char*)(_stack_content(stack) + stack->capacity)))
            status |= STACK_BR_CANARY_FAIL;
    })

    ON_HASH(if (stack->_hash != _stack_hash(stack)) status |= STACK_HASH_FAILURE);

    return status;
}

bool stack_check_canary(const stack_canary_t value) {
    return !memcmp(value, STACK_CANARY_VALUE, sizeof(stack_canary_t));
}

void _stack_dump(Stack* const stack, int importance, const char* function, const size_t line, const char* file) {
    _log_printf(importance, "dump", " ----- Stack dump in function %s of file %s (%ld): ----- \n", function, file, line);

    stack_report_t status = stack_status(stack);
    _log_printf(importance, "dump", "\tStatus: %s\n", status ? "CORRUPT" : "OK");
    for (int error_id = 0; error_id < (int)sizeof(STACK_STATUS_DESCR) / (int)sizeof(STACK_STATUS_DESCR[0]); ++error_id) {
        if (status & (1 << error_id)) {
            _log_printf(importance, "dump", "\t\t%s\n", STACK_STATUS_DESCR[error_id]);
        }
    }

    _log_printf(importance, "dump", "\tStack at %p:\n", stack);
    if (check_ptr(stack) == false) return;

    ON_CANARY(_log_printf(importance, "dump", "\t\tLeft canary  = \"%6s\"\n", stack->_canary_left));
    ON_CANARY(_log_printf(importance, "dump", "\t\tRight canary = \"%6s\"\n", stack->_canary_right));
    _log_printf(importance, "dump", "\t\tCapacity     = %ld\n", stack->capacity);
    _log_printf(importance, "dump", "\t\tSize         = %ld\n", stack->size);
    _log_printf(importance, "dump", "\t\tBuffer       = %p\n", stack->buffer);
    _log_printf(importance, "dump", "\t\t\t[----] = \"%6s\"\n", stack->buffer);

    int limit = stack->capacity < STACK_DUMP_MAX_LINES ? 
                stack->capacity : STACK_DUMP_MAX_LINES;
    const int STACK_BITES_PER_LINE = sizeof(stack_content_t) < STACK_DUMP_MAX_BITES ? 
                                     sizeof(stack_content_t) : STACK_DUMP_MAX_BITES;

    for (int elem_id = 0; elem_id < limit; ++elem_id) {
        stack_content_t* elem_start = _stack_content(stack) + elem_id;
        char biteline[STACK_BITES_PER_LINE * 9 + 1];
        size_t end_index = 0;

        for (int bite_id = 0; bite_id < STACK_BITES_PER_LINE; ++bite_id) {
            end_index += sprintf(biteline + end_index, "0x%02X ", *((char*)elem_start + bite_id) & 0xff);
        }
        for (int bite_id = 0; bite_id < STACK_BITES_PER_LINE; ++bite_id) {
            char bite = *((char*)elem_start + bite_id);
            end_index += sprintf(biteline + end_index, "%c ", isprint(bite) ? bite : '.');
        }

        biteline[sizeof(biteline) - 1] = '\0';

        _log_printf(importance, "dump", "\t\t\t[%04d] = (%-6s) at %p: %s\n", elem_id,
            *elem_start == STACK_CONTENT_POISON ? "POISON" : "VALUE", _stack_content(stack) + elem_id, biteline);
    }

    _log_printf(importance, "dump", "\t\t\t[####] = \"%6s\"\n", 
                (char*)(_stack_content(stack) + stack->capacity));
    ON_HASH(_log_printf(importance, "dump", "\t\tHash      = %ld\n", stack->_hash));
    ON_HASH(_log_printf(importance, "dump", "\t\tEst. hash = %ld\n", _stack_hash(stack)));
}

void _stack_change_size(Stack* const stack, const size_t new_size, int* const err_code) {
    _LOG_FAIL_CHECK_(!stack_status(stack), "error", ERROR_REPORTS, return, err_code, EINVAL);

    char* new_buffer = _stack_alloc_space(new_size, err_code);
    _LOG_FAIL_CHECK_(new_buffer, "error", ERROR_REPORTS, return, err_code, ENOMEM);

    memcpy(new_buffer, stack->buffer, new_size * sizeof(stack_content_t) + ((char*)_stack_content(stack) - stack->buffer));

    free(stack->buffer);
    stack->buffer = new_buffer;
    stack->capacity = new_size;

    ON_HASH(stack->_hash = _stack_hash(stack));
}

char* _stack_alloc_space(const size_t count, int* const err_code) {
    size_t prefix_size = sizeof(stack_canary_t) + 
        (alignof(stack_content_t) - sizeof(stack_canary_t) % alignof(stack_content_t)) % sizeof(stack_content_t);
    size_t buffer_size = prefix_size * 2 + sizeof(stack_content_t) * count;
                                    /* ^-- two canaries */

    char* buffer = (char*)calloc(buffer_size, sizeof(char));
    _LOG_FAIL_CHECK_(buffer, "error", ERROR_REPORTS, return NULL, err_code, ENOMEM);

    strncpy(buffer,                                        STACK_CANARY_VALUE, sizeof(stack_canary_t));
    strncpy(buffer + buffer_size - prefix_size, STACK_CANARY_VALUE, sizeof(stack_canary_t));

    stack_content_t* beginning = (stack_content_t*)(buffer + prefix_size);
    for (size_t id = 0; id < count; ++id) {
        *(beginning + id) = STACK_CONTENT_POISON;
    }

    return buffer;
}

stack_content_t* _stack_content(const Stack* const stack) {
    size_t prefix_size = sizeof(stack_canary_t) + 
        (alignof(stack_content_t) - sizeof(stack_canary_t) % alignof(stack_content_t)) % sizeof(stack_content_t);
    return (stack_content_t*)(stack->buffer + prefix_size);
}

stack_hash_t _stack_hash(const Stack* const stack) {
    const char* stack_end = (const char*)(&stack->capacity + 1);
    ON_CANARY(stack_end -= alignof(stack_canary_t));
    ON_HASH(stack_end -= alignof(stack_hash_t));
    hash_t hash = get_hash(stack, stack_end);
    if (check_ptr(stack->buffer)) {
        hash += get_hash(stack->buffer, (char*)(_stack_content(stack) + stack->capacity) + 
                                        ((char*)_stack_content(stack) - stack->buffer));
    }
    return hash;
}

#endif

