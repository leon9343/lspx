#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/error.h"
#include "./include/file.h"
#include "./include/parse.h"
#include "./include/compile.h"

// ERRORS //////////////////////////////////
new_error(FEW_ARGS, FILE_ERROR, "too few arguments, use ./lpsx <script> <target .c file>");
new_error(NO_FILE, FILE_ERROR, "no input file was given");
////////////////////////////////////////////

int main(int argc, char** argv) {
	if(argc < 3) 
		if(error(FEW_ARGS))
			exit(1);

	char* file_path = argv[1];

	char* src_code = read_file(file_path);

	if(!src_code)
		if(error(NO_FILE))
			exit(2);

	PARSER* parser = (PARSER*)malloc(sizeof(PARSER));
	init_parser(parser);
	parse(parser, src_code);

	char* c_file;
	COMPILER* compiler = (COMPILER*)malloc(sizeof(COMPILER));
	init_compiler(compiler);
	c_file = compile(compiler, parser);

	write_file(argv[2], c_file);

	free(parser);
	free(compiler);

	system("make psx");

	return 0;
}
