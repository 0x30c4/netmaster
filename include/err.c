#include "err.h"

const char STATUS_CODE[7][26] = {
					"200 OK\0", "400 Bad Request\0", "401 Unauthorized\0", "403 Forbidden\0",
					"404 Not Found\0", "406 Not Acceptable\0", "500 Internal Server Error\0"
					};

const char ERROR_TYP_TXT[7][32] = {
								   "Every thing is ok :-)",
								   "This request is a bad request!!\0", 
								   "Unauthorized to access\0",
								   "This resource is forbidden:(\0",
								   "Can't locate the resource :/\0",
								   "Request is not acceptable!#@!\0",
								   "Server error, [Internal]!!@#$!!\0"
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

void *errorPageSender(int client_socket, int error_code, char file_name[BUFSIZE]){
	char buf[1];
	char tmp[BUFSIZE / 2];
	size_t bytes_read;

	bzero(&tmp, BUFSIZE / 2);
	bzero(&buf, 1);

    if(realpath(ERROR_PAGE, tmp) == NULL){
    	printf("ERROR(bad path): %s\n", buf);
    	return NULL;
    }

    FILE *file = fopen(tmp, "r");
    // FILE *file = fopen(actualpath, "r");
    if(file == NULL){
    	fprintf(stderr, "ERROR(open): %s\n", tmp);
    	return NULL;
    }

	bzero(&tmp, sizeof(tmp));
	
	unsigned int size = 0;

    int count = 0;
    int count2 = 0;
    int starting_pos[10];
    short starting_pos_typ[10];
    bool m = TRUE;
    bool s = FALSE;

    for (int i = 0; i < 10; ++i){
    	starting_pos[i] = -1;
    	starting_pos_typ[i] = -1;
    }
    char ERROR_PAGE_VAR[2][11] = {"ERROR_NAME\0", "ERROR_TEXT\0"};
    while((bytes_read = fread(buf, 1, 1, file)) > 0){
		if (ERROR_PAGE_VAR[0][0] == buf[0] || ERROR_PAGE_VAR[1][0] == buf[0])
			s = TRUE;

    	if ((ERROR_PAGE_VAR[0][count] == buf[0] || ERROR_PAGE_VAR[1][count] == buf[0]) && s ){
    		count++;
    		m = TRUE;
    	}else
    		m = FALSE;

    	if (m) strncat(tmp, buf, sizeof(buf));
    	
    	// printf("%d", strlen(tmp));
    	if (count == 10 && strlen(tmp) == 10 && count2 < 10) {
    		starting_pos[count2] = ftell(file) - 10;
    		
    		starting_pos_typ[count2] = (stringcmp(ERROR_PAGE_VAR[0], tmp)) ? 0 : 1;

    		bzero(&tmp, sizeof(tmp));
    		count = 0;
    		count2++;
    		m = FALSE;
    	}
    }

    rewind(file);
    count = 0;
    count2 = 0;
    while((bytes_read = fread(buf, 1, 1, file)) > 0){
    	size += bytes_read;
    	for (int i = 0; i < 10; ++i){
    		if (starting_pos[i] == -1) break;
    		if (starting_pos_typ[i] == -1) break;
    		if (starting_pos[i] == (size - 1)) {
    			m = TRUE;
    			count2 = i;
    			break;
    		}
    	}
    	if (m && count == 0){
    		if (starting_pos_typ[count2]){
    			if (error_code >= 2 && error_code <= 4)
    				dprintf(client_socket, "%s <br>\"%s\"", ERROR_TYP_TXT[error_code], file_name);
    			else
    				dprintf(client_socket, "%s", ERROR_TYP_TXT[error_code]);
    		}
    		else
    			dprintf(client_socket, "%s", STATUS_CODE[error_code]);

    	}
 		if (count == 10) {
 			m = FALSE;
 			count = 0;
 		}
 		if (m) count++;

    	if (!m){
            write(client_socket, buf, sizeof(buf));
        }
    }
    dprintf(client_socket, "%s", EOH);
    fclose(file);
    return NULL;
}
