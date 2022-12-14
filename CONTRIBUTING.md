# Contributing to Text Sorter program
## General information
First of all, thank you for contributing to this wasteland of a project. 
Second af all, it was not expected that anyone will ever make contributions to this. 
And, finally, if you still want to commit changes, make sure you read README.md and CODE_OF_CONDUCT.md 
as these files contain some important information about how you should not be an a*shole.
## Language
Primarely English (US). It also applyes to comments, documentation, naming and messages.
There are a few (2 lines in main.cpp) comments written in Russian, though, as they describe 
joke function and not very important, and it is highly exceptional and should not be present anywhere else in the code.
It also will be highly appreciated if you spot grammatical mistakes anywhere in the content of this reposiory and will be able to fix them.
## Code Style
We use codestyle that is close to pep8 for python, but for C++.

### Naming
Variables and functions should be names in lower case.
```
void example_function(int example_variable = 1);
```
Classes and structures should be names in cammel case, starting with capital letter.
```
struct ExampleStruct {
    const char* first_name = "";
    const char* last_name = "";
};

class ExampleClass {
private:
    int salary = 120000;
public:
    int age = 24;
    struct ExampleStruct name = {
        first_name = "Owl", 
        last_name = "Hooter"
    };
};
```
Macros should be names in upper case, starting and ending with "_". If macro features multi-line definitions, "\\" symbols should be alligned vertically on first tabulation position after the longest line of definition.
```
#define _EXAMPLE_DEFINE_(...) do {                  \
    printf("Your formula will be printed here:\n"); \
    printf("%s\n", #__VA_ARGS__);                   \
} while(0)
```
Constants (including macro-defines ones) and check-defines should be names in upper case.
```
#ifndef CONSTANTS_MODULE_H
#define CONSTANTS_MODULE_H

#define PI 3.1415926535

const double SURFACE_G = 9.815;

#endif
```
Code files and headers should be names in lower case.
```
#include "my_module.h"
```
Git-specific files should be names in upper case
```
FILE* file = fopen("LICENSE", "r");
```
Single-character variables are only allowed in function declarations

module.h
```
void some_function(const int a, const int b);
```
module.cpp
```
void some_function(const int coef_a, const int coef_b) {
    ...
}
```

### Comments
Line comments should be written as
```
int owl_age = 3;  // Age of the owl.
```
Dev comments should be written as
```
// General-purpose comment.

// TODO: Tasks that should be done later.

//* TODO: Tasks that were done, but not checked.

//! TODO: Task that can not or should not be done.
//! Reason why this specific task can not be done.

//! Bugs, warnings and other 
//! bug-related important information.

//* Important notes.

int acceleration = SURFACE_G * sin(1.25);
//                      descriptions ^

//// Unimportant comments.
```
Function, class, structure, macro and file descriptions should be written compatible with the newest doxygen standart and marked as
```
/**
 * @brief
 * Briefs should be written below brief tag
 * if they are too long for one line.
 *
 * @param number_of_owls general argument or memeber
 *
 * @param[in] input_param function input argument 
 *                        (if out argument is present)
 * @param[out] function output argument
 *
 * @note Small briefs/notes can be done in single line.
 *
 * @author Author Name (author_email@corp.com)
 */
```
### Line transitions
Code blocks (curly brackets) should be written as
```
void function() {
    // code
}

struct Structure {
    // structure members
};
```
Excepts what defining **short** structures or arrays
```
const char* crew = {"Mike", "Threvor", "Franklin"};
struct Person leader = {"Phil", "Bobson"};
```
Long structures and arrays should be defined as code blocks and arguments should be alligned if it is required.
```
double matrix[9] = {
    10.0, 0.0,  0.0,
    0.0,  10.0, 0.0,
    0.0,  0.0,  10.0,
};

long indices[8] = {
    1234, 4321, 7674,
    1987, 7168, 3415,
    3073, 112
};

struct Animal owl = {
    .can_fly = true,
    .can_swim = false,
    .action = hoot,
    .sound = {"Hoot", "Huit", "*flop*"}
};
```

If one formula is too long for one line and **can not be split into multiple formulas**, then it should be split into multiple blocks according to level of brackets.
```
double some_complex_value = (number_of_bats + number_of_caves -
                                number_of_people + (number_of_cages - 
                                    number_of_parrots) * 3) / 
                                (cargo_divisor + 1);
```
## Project Structure
**stackworks** - library implementing stack data structure. It is essential to define ```stack_content_t``` (type of elements that should be stored in a stack) and ```stack_content_t STACK_CONTENT_POISON``` (value that will be put into empty cells of the stack).

**logger** - module that creates and manages program logs. ```log_init()``` initializes log files, ```log_close()``` closes them and ```log_printf()``` prints lines into logs with all the formating.

**debug** - module for easier debugging. It contains function ```end_program()``` that is not very agile, but is used by 

**argparser** - module for parsing command line arguments. Used only by **main.cpp**, but is very agile and can be helpful for any program that should read command line arguments.

**utils** - module with "orphan" functions.

**main.cpp** - entry point of the program. When ran it functions as a console for stack operations.
## Logger Structure
**logger** module, when initialized through ```log_init()``` function, creates file that later would be filled with logs and defines certaint importance thrashold that would prevent less important messages (like status reports) from filling the log file. When function ```log_printf()``` is called, it receives importance level of a message to print, and, if that importance is less then logger threshold, ignores the message.
## Contact Information
For more information about contributing to the project contact

***kudriashov.it@phystech.edu***