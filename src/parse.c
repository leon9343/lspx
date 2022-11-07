#include "./include/parse.h"

void init_parser(PARSER* p) {
    for(int i=0; i < PRG_SIZE; i++) {
        p->program[i] = NULL;
    }
}

// TODO: add some error checking
void parse(PARSER* p, char* buffer) {
	char* program = strdup(buffer);
	char* delim = " \n";

	char* token = strtok(program, delim);
	int i = 0;

	while(token) {
		p->program[i] = token;
		i++;
		token = strtok(NULL, delim);
	}
}
