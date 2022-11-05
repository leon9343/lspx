#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/error.h"
#include "./include/file.h"
#include "./include/parse.h"
#include "./include/compile.h"

// ERRORS //////////////////////////////////
new_error(FEW_ARGS, FILE_ERROR, "too few arguments");
new_error(NO_FILE, FILE_ERROR, "no input file was given");
////////////////////////////////////////////

int main(int argc, char** argv) {
	if(argc < 2) 
		if(error(FEW_ARGS))
			exit(1);

	char* file_path = argv[1];

	char* src_code = read_file(file_path);

	if(!src_code)
		if(error(NO_FILE))
			exit(2);

	PARSER parser;
	init_parser(&parser);

	parse(&parser, src_code);

	char* c_file;
	// COMPILER* compiler = (COMPILER*)malloc(sizeof(COMPILER));
	COMPILER compiler;
	init_compiler(&compiler);
	c_file = compile(&compiler, &parser);

	// FIXME: add as command line argument
	write_file("psx.c", c_file);

	int i=1;
	if(i)
		system("make psx");

	return 0;
}
