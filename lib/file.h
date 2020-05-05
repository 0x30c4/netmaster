#include "common.h"

void file_reader(char *fn, int fd){
	struct stat sb;
	if (lstat(fn, &sb) == -1) {
		perror("netpussy");
        // exit(EXIT_FAILURE);
    }else{
		FILE *f = fopen(fn, "r");
		while(!feof(f)){
			// write(fd, (char*)fgetc(f),1);
			printf("%c", fgetc(f));
		}
		write(fd, "\n", 1);
		fclose(f);
    }
}