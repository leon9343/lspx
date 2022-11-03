#include "./include/error.h"

int error(ERROR err) {
	int ret = 1;
	switch(err.type) {
		case GENERIC_ERROR:
			printf("ERROR: ");
			break;
		case FILE_ERROR:
			printf("FILE ERROR: :");
			break;
		case WARNING:
			printf("WARNING: ");
			ret = 0;
			break;
	}
	printf("%s\n", err.msg);
	return ret;
}
