#pragma once
#include <stdlib.h>
#include "error.h"


long size_file(FILE* file);

char* read_file(char* path);

void write_file(char* path, char* buffer);