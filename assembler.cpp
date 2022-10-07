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
#include <fcntl.h>
#include <unistd.h>

#include "lib/util/dbg/debug.h"
#include "lib/util/argparser.h"
#include "lib/file_proc.h"
#include "lib/processor/procinfo.h"
#include "lib/processor/proccmd.h"

/**
 * @brief Get the input file name from the list of command line arguments.
 * 
 * @param argc argument count
 * @param argv argument values
 * @return const char* 
 */
const char* get_input_file_name(const int argc, const char** argv);

/**
 * @brief Get the output file name from the list of command line arguments.
 * 
 * @param argc argument count
 * @param argv argument values
 * @return const char* 
 */
const char* get_output_file_name(const int argc, const char** argv);

// Ignore everything less or equaly important as status reports.
static int log_threshold = STATUS_REPORTS + 1;
static int gen_listing = 1;

static const int NUMBER_OF_OWLS = 10;

static const int NUMBER_OF_TAGS = 3;
static const struct ActionTag LINE_TAGS[NUMBER_OF_TAGS] = {
    {
        .name = {'O', "owl"}, 
        .action = {
            .parameters = {}, 
            .parameters_length = 0, 
            .function = print_owl, 
        },
        .description = "print 10 owls to the screen."
    },
    {
        .name = {'I', ""}, 
        .action = {
            .parameters = (void*[]) {&log_threshold},
            .parameters_length = 1, 
            .function = edit_int,
        },
        .description = "set log threshold to the specified number.\n"
                        "\tDoes not check if integer was specified."
    },
    {
        .name = {'L', ""}, 
        .action = {
            .parameters = (void*[]) {&gen_listing},
            .parameters_length = 1, 
            .function = edit_int,
        },
        .description = "set if program should generate listing or not.\n"
                        "\tDoes not check if integer was specified."
    },
};

const char* DEFAULT_OUTPUT_NAME = "a.instr";

int main(const int argc, const char** argv) {
    atexit(log_end_program);

    parse_args(argc, argv, NUMBER_OF_TAGS, LINE_TAGS);
    log_init("program_log.log", log_threshold, &errno);
    print_label();

    const char* file_name = get_input_file_name(argc, argv);
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, {
        printf("Input file was not specified, terminating...\n");
        printf("To execute program stored in a file run\n%s [file name]\n", argv[0]);
        return EXIT_FAILURE;
    }, NULL, 0);

    const char* out_name = get_output_file_name(argc, argv);
    if (out_name == NULL) {
        out_name = DEFAULT_OUTPUT_NAME;
    }

    //* We need bufferization for windows capability (sigh...)
    FILE* input = fopen(file_name, "r");
    _LOG_FAIL_CHECK_(input, "error", ERROR_REPORTS, {
        log_printf(ERROR_REPORTS, "error", "Failed to open input file \"%s\", terminating...\n", file_name);
        return EXIT_FAILURE;
    }, NULL, 0);
    setvbuf(input, NULL, _IOFBF, flength(fileno(input)));

    FILE* output = fopen(out_name, "wb");
    _LOG_FAIL_CHECK_(output, "error", ERROR_REPORTS, {
        log_printf(ERROR_REPORTS, "error", "Failed to create/open output file \"%s\", terminating...\n", out_name);
        return EXIT_FAILURE;
    }, NULL, 0);

    FILE* listing = NULL;
    if (gen_listing) {
        listing = fopen("listing.txt", "wb");
        _LOG_FAIL_CHECK_(output, "warning", WARNINGS, {
            log_printf(WARNINGS, "warning", "Failed to create/open listing file \"%s\". "
                                            "No listing will be generated.\n", out_name);
        }, NULL, 0);
    }

    
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
    printf("Assembler for a stack-based processor by Ilya Kudryashov.\n");
    printf("Program assembles human-readable code into binary program.\n");
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n", __DATE__, __TIME__);
}

const char* get_input_file_name(const int argc, const char** argv) {
    const char* file_name = NULL;

    for (int argument_id = 1; argument_id < argc; ++argument_id) {
        if (*argv[argument_id] == '-') continue;
        file_name = argv[argument_id];
        break;
    }

    return file_name;
}

const char* get_output_file_name(const int argc, const char** argv) {
    const char* file_name = NULL;

    bool enc_first_name = false;
    for (int argument_id = 1; argument_id < argc; ++argument_id) {
        if (*argv[argument_id] == '-') continue;
        file_name = argv[argument_id];
        if (enc_first_name) return file_name;
        else enc_first_name = true;
    }

    return NULL;
}