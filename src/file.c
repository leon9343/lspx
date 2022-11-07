#include "./include/file.h"

// ERRORS ///////////////////////////////////
new_error(FOPEN, FILE_ERROR, "failed to open file (fopen())");
new_error(FREAD, FILE_ERROR, "failed to read file (fread())");
new_error(FGETPOS, WARNING, "fgetpos() failed on file");
new_error(FSETPOS, WARNING, "fsetpos() failed on file");
////////////////////////////////////////////

long size_file(FILE* file) {
	// i check if file is empty beforehand
	fpos_t org;

	if(fgetpos(file, &org) != 0)
		if(error(FGETPOS, ""))
			exit(1);
	
	fseek(file, 0, SEEK_END);
	long out = ftell(file);

	if(fsetpos(file, &org) != 0)
		if(error(FSETPOS, ""))
			exit(1);

	return out;
}

char* read_file(char* path) {
	FILE* file = fopen(path, "r");

	if(!file) {
		free(file);
		if(error(FOPEN, ""))
			exit(1);
	}

	long size = size_file(file);
	char* buffer = malloc(size + 1);
	
	size_t num_of_bytes = fread(buffer, 1, size, file);

	if(num_of_bytes != size) {
		free(buffer);
		if(error(FREAD, ""))
			exit(1);
	}

	// make sure that there is an EOF
	buffer[size] = '\0';

	return buffer;
}

void write_file(char* path, char* buffer) {
	FILE* file = fopen(path, "w");

	if(!file) {
		free(file);
		if(error(FOPEN, ""))
			exit(1);
	}

	fprintf(file, "%s", buffer);

	fclose(file);
}