/**
 * @file stackreports.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Stack status report tags.
 * @version 0.1
 * @date 2022-09-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef STACK_REPORTS_H
#define STACK_REPORTS_H

typedef int stack_report_t;
enum STACK_STATUSES {
    STACK_NULL = 1 << 0,
    STACK_BIG_SIZE = 1 << 1,
    STACK_NULL_CONTENT = 1 << 2,
    STACK_L_CANARY_FAIL = 1 << 3,
    STACK_R_CANARY_FAIL = 1 << 4,
    STACK_BL_CANARY_FAIL = 1 << 5,
    STACK_BR_CANARY_FAIL = 1 << 6,
    STACK_HASH_FAILURE = 1 << 7,
};

#endif