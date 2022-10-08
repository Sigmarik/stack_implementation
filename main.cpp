/**
 * @file main.c
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Program for reading text file and sorting its lines.
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clocale>
#include <ctype.h>

#include "lib/util/dbg/debug.h"
#include "lib/util/argparser.h"

#include "lib/ll_stack.h"

/**
 * @brief Print a bunch of owls.
 * 
 * @param argc unimportant
 * @param argv unimportant
 * @param argument unimportant
 */
void print_owl(const int argc, void** argv, const char* argument);

/**
 * @brief Print program label and build date/time to console and log.
 * 
 */
void print_label();

/**
 * @brief Print prefix and read an integer.
 * 
 * @param prefix prefix to print before each request
 * @param variable pointer to the variable to push the result
 * @param err_code variable to fill with error code
 */
void read_ld(const char* prefix, ll_stack_content_t* variable, int* err_code = NULL);

/**
 * @brief Print prefix and read one-character command.
 * 
 * @param prefix prefix to print before each request
 * @param commands lost of available commands
 * @return char
 */
char read_command(const char* prefix, const char* commands);

/**
 * @brief Print the list of available commands.
 * 
 */
void print_commands();

/**
 * @brief Execute user's single-character command on a stack.
 * 
 * @param stack execution subject
 * @param command one-char user input
 * @param err_code variable to fill with error code
 */
void execute_user_command(LLStack stack, bool* const runtime_status, const char command, int* const err_code = NULL);

// Ignore everything less or equaly important as status reports.
static int log_threshold = STATUS_REPORTS + 1;

static const int NUMBER_OF_OWLS = 10;

static const int NUMBER_OF_TAGS = 2;
static const struct ActionTag LINE_TAGS[NUMBER_OF_TAGS] = {
    {
        .name = {'O', "owl"}, 
        .action = {
            .parameters = {}, 
            .parameters_length = 0, 
            .function = print_owl, 
        },
        .description = "prints 10 owls on the screen."
    },
    {
        .name = {'I', ""}, 
        .action = {
            .parameters = (void*[]) {&log_threshold},
            .parameters_length = 1, 
            .function = edit_int,
        },
        .description = "sets log threshold to the specified number.\n"
                        "\tDoes not check if integer was specified."
    },
};

int main(const int argc, const char** argv) {
    atexit(log_end_program);

    parse_args(argc, argv, NUMBER_OF_TAGS, LINE_TAGS);
    log_init("program_log.log", log_threshold, &errno);
    print_label();

    const size_t RQ_PREFIX_SIZE = 512;
    char request_prefix[RQ_PREFIX_SIZE] = "";
    strncat(request_prefix, argv[0], sizeof(request_prefix) - 2);
    strcat(request_prefix, "# ");

    log_printf(STATUS_REPORTS, "status", "Creating stack...\n");
    LLStack stack = ll_stack_ctor(4, &errno);
    if (ll_stack_status(stack)) {
        log_printf(ERROR_REPORTS, "error", "Stack is invalid after initialization, terminating.\n");
        ll_stack_dump(stack, ERROR_REPORTS);
        return EXIT_FAILURE;
    }
    log_printf(STATUS_REPORTS, "status", "Stack initialized, entering main loop...\n");

    print_commands();

    bool program_alive = true;
    while (program_alive) {

        execute_user_command(stack, &program_alive, read_command(request_prefix, "HQPRGD"), &errno);

        log_printf(STATUS_REPORTS, "status", "Stack status check started...\n");

        _LOG_FAIL_CHECK_(ll_stack_status(stack) == 0, "ERROR", ERROR_REPORTS, {
            puts("Stack failed, terminating.");
            log_printf(ERROR_REPORTS, "error", "Stack failed, terminating.\n");
            ll_stack_dump(stack, ERROR_REPORTS);
            return EXIT_FAILURE;
        }, &errno, EINVAL);

        log_printf(STATUS_REPORTS, "status", "Tick ended successfully.\n");
        ll_stack_dump(stack, STATUS_REPORTS);
    }

    ll_stack_dtor(stack);

    return EXIT_SUCCESS;
}

// Офигенно, ничего не менять.
// Дополнил сову, сорри.
void print_owl(const int argc, void** argv, const char* argument) {
    printf("-Owl argument detected, dropping emergency supply of owls.\n");
    for (int index = 0; index < NUMBER_OF_OWLS; index++) {
        puts(R"(    A_,,,_A    )");
        puts(R"(   ((O)V(O))   )");
        puts(R"(  ("\"|"|"/")  )");
        puts(R"(   \"|"|"|"/   )");
        puts(R"(     "| |"     )");
        puts(R"(      ^ ^      )");
    }
}

void print_label() {
    printf("Stack implementation by Ilya Kudryashov.\n");
    printf("Program implements stack data structure.\n");
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n", __DATE__, __TIME__);
}

void read_ld(const char* prefix, ll_stack_content_t* variable, int* err_code) {
    _LOG_FAIL_CHECK_(prefix && variable, "warning", WARNINGS, return;, err_code, EFAULT);

    printf("%s", prefix);
    while (!scanf("%ld", (long int*)variable)) {
        printf("Integer argument expected.\n");
        printf("%s", prefix);
    }

    while (getc(stdin) != '\n') {};
}

char read_command(const char* prefix, const char* commands) {
    printf("%s", prefix);

    char action = '\0';
    while ((action = getc(stdin)) != EOF) {

        int err_cnt = 0;
        char last_char = action;
        while (action != '\n' && (last_char = getc(stdin)) != '\n')
            if (isprint(last_char)) ++err_cnt;

        bool match = false;
        for (const char* pointer = commands; *pointer != '\0'; ++pointer) {
            match = match || (*pointer == action);
        }

        if (match) break;

        printf("Expected single character from the list.\n%s", prefix);
    }

    return action;
}

void print_commands() {
    printf("List of actions:\n"
            "H - print this list\n"
            "Q - quit program\n"
            "P - push element to the stack\n"
            "R - remove last element of the stack\n"
            "G - get last element in the stack\n"
            "D - dump stack information to logs\n");
}

void execute_user_command(LLStack stack, bool* const runtime_status, const char command, int* const err_code) {
    log_printf(STATUS_REPORTS, "status", "Processing command %c.\n", command);

    ll_stack_content_t argument = 0;
    switch (command) {
        case 'H': { print_commands(); break; }

        case 'Q': { *runtime_status = false; break; }

        case 'P':
            read_ld("Value to push to the stack -> ", &argument);
            log_printf(STATUS_REPORTS, "status", "Command argument = %ld.\n", argument);
            ll_stack_push(stack, argument, &errno);
            break;

        case 'R':
            if (ll_stack_size(stack) == 0) {
                printf("Stack is empty.\n");
                break;
            }
            ll_stack_pop(stack, &errno);
            break;

        case 'G':
            if (ll_stack_size(stack) == 0) {
                printf("Stack is empty.\n");
                break;
            }
            printf("Last element of the stack -> %ld.\n", (long int)ll_stack_pull(stack, &errno));
            break;

        case 'D':
            log_printf(ABSOLUTE_IMPORTANCE, "dump", "Stack encrypted address: %p\n", stack);
            ll_stack_dump(stack, ABSOLUTE_IMPORTANCE);
            printf("Stack was dumped into logs.\n");
            break;

        default:
            puts("Failed to read command.");
            log_printf(WARNINGS, "warning", "Failed to identify command %c.\n", command);
            break;
    }

    log_printf(STATUS_REPORTS, "status", "Processing of the command %c ended.\n", command);
}