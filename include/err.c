#include "err.h"

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