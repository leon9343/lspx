#pragma once
#include <stdlib.h>
#include "error.h"
#include "parse.h"

/**
 * Starting from parser.program I have an array of tokens which contain useful declarations.
 * Now, lets make a list of functions that should work:
 *      - ; This starts a comment. if program[i] == ";" -> ignore untill program[i] == "\n"
 *      - def This defines a var. if program[i] == "def" -> variable.type = program[i+1]
 *                                                          variable.name = program[i+2]
 *                                                          i += 3
 *      - init This initializes a sys. if program[i] == "init" -> if program[i+1] == "graphics"
 *                                                                      setgraphics flag
*/

#define SIZE 100

typedef struct {
    char* type;
    char* name;
    int status;
}VARIABLE;

typedef struct {
    char* fun;
    char* target;
    char* args[SIZE];
}FUNCTION;

typedef struct {
    int graph_flag;

    // TODO: add in functions later
    VARIABLE variables[SIZE];
    FUNCTION functions[SIZE];
}COMPILER;

void init_compiler(COMPILER* c);
char* compile(COMPILER* c, PARSER* p);

void type_check(char* buf);
void args_check(char* buf, int argno);
int fun_check(char* buf);