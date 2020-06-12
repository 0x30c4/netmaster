#include "err.h"

const char STATUS_CODE[7][26] = {
					"200 OK\0", "400 Bad Request\0", "401 Unauthorized\0", "403 Forbidden\0",
					"404 Not Found\0", "406 Not Acceptable\0", "500 Internal Server Error\0"
					};

int check(int exp, const char *msg){
	if (exp == SOCKETERROR){
		fprintf(stderr, "%s%s%s", RED, UNDERLINE, BOLD);
		perror(msg);
		fprintf(stderr, "%s", RESETALL);
		exit(1);
	}
	return exp;
}

// int checkIfNull(void *exp, const char *msg){
// 	// if (*((void*)exp) == NULL){
// 	if (exp == NULL){
// 		perror(msg);
// 		return 0;
// 	}
// 	return 1;
// }