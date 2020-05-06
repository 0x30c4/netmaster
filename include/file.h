#include "common.h"

void file_reader(char *fn, int fd){
	
    printf("t111111111111111111");

	FILE* f = fdopen(fd, "w");
    
	struct stat sb;
	if (lstat(fn, &sb) == -1) {
		perror("netpussy");
        // exit(EXIT_FAILURE);
    }else{
		FILE *f = fopen(fn, "r");
		while(!feof(f)){
			write(fd, fgetc(f), 1);
			// printf("%c", fgetc(f));
		}
		write(fd, "\n", 1);
		fclose(f);
    }
}
