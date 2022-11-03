// int a 5;
// function b (args) (function);
// print(arg);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/error.h"
#include "./include/file.h"

// ERRORS //////////////////////////////////
new_error(FEW_ARGS, FILE_ERROR, "too few arguments");
new_error(NO_FILE, FILE_ERROR, "no input file was given");
////////////////////////////////////////////

int main(int argc, char** argv) {
	if(argc < 2) 
		if(error(FEW_ARGS))
			exit(1);

	char* file_path = NULL;

	for(int i = 0; i < argc; i++) {
		if(argv[i] == "-f") {
			strcpy(file_path, argv[i+1]);
			i++;
		}
	}

	char* src_code = read_file(file_path);

	if(!src_code)
		if(error(NO_FILE))
			exit(2);

	// PARSER p_status = parse(argv[1]);

	printf("%s\n", src_code);

	return 0;
}
