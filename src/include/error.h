#pragma once
#include <stdio.h>

#define new_error(n, t, m) ERROR (n) = { t, m }

typedef struct {
	enum type {
		GENERIC_ERROR,	
		FILE_ERROR,
		WARNING
	} type;
	char* msg;
} ERROR;

int error(ERROR err);
