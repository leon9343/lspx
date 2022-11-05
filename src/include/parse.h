#pragma once
#include <string.h>
#include "error.h"

/**
 *  char* BUFFER -> working C code
 * 
 *  1) Have an array of variables (add an array of function buffers later)
 *     So, starting from my buffer, when I see the word "def", say at index i, I know that
 * 	   at i+1 I can find the type and at i+2 I can find the name of the variable.
*/

#define PRG_SIZE  1000

typedef struct {
	// TODO: add more later (functions, syntax errors etc.)
	char* program[PRG_SIZE];
} PARSER;

void init_parser(PARSER* p);

void parse(PARSER* p, char* buffer);