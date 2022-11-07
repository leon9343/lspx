#include "./include/error.h"

int error(ERROR err, char* extra) {
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
	printf("%s - %s\n", err.msg, extra);
	return ret;
}
