#include "../include/err.h"

char STATUS_CODE_OK[7] = "200 OK\0";
char STATUS_CODE_Bad_Request[16] = "400 Bad Request\0";
char STATUS_CODE_Unauthorized[17] = "401 Unauthorized\0";
char STATUS_CODE_Forbidden[14] = "403 Forbidden\0";
char STATUS_CODE_Not_Found[14] = "404 Not Found\0";
char STATUS_CODE_Not_Acceptable[19] = "406 Not Acceptable\0";
char STATUS_CODE_Request_URI_Too_long[17] = "414 Too Long URI\0";
char STATUS_CODE_Internal_Server_Error[26] = "500 Internal Server Error\0";

char STATUS_MSG_OK[22] = "Every thing is ok :-)\0";
char STATUS_MSG_Bad_Request[14] = "Bad request!!\0";
char STATUS_MSG_Unauthorized[24] = "Unauthorized to access.\0";
char STATUS_MSG_Forbidden[29] = "This resource is forbidden:(\0";
char STATUS_MSG_Not_Found[28] = "Can't locate the resource:/\0";
char STATUS_MSG_Not_Acceptable[28] = "Request is not acceptable:/\0";
char STATUS_MSG_Request_URI_Too_long[23] = "Request-URI Too Long:/\0";
char STATUS_MSG_Internal_Server_Error[24] = "Internal Server Error:/\0";

char *HeaderErrNo(int err){
    switch (err){
    case OK:
        return STATUS_MSG_OK;

    case Bad_Request:
        return STATUS_MSG_Bad_Request;

    case Unauthorized:
        return STATUS_MSG_Unauthorized;

    case Forbidden:
        return STATUS_MSG_Forbidden;

    case Not_Found:
        return STATUS_MSG_Not_Found;

    case Not_Acceptable:
        return STATUS_MSG_Not_Found;

    case Request_URI_Too_long:
        return STATUS_MSG_Request_URI_Too_long;

    case Internal_Server_Error:
        return STATUS_MSG_Internal_Server_Error;

    default:
        return NULL;
    }
}

char *HeaderErrNoStatusCode(int err){
    switch (err){
    case OK:
        return STATUS_CODE_OK;

    case Bad_Request:
        return STATUS_CODE_Bad_Request;

    case Unauthorized:
        return STATUS_CODE_Unauthorized;

    case Forbidden:
        return STATUS_CODE_Forbidden;

    case Not_Found:
        return STATUS_CODE_Not_Found;

    case Not_Acceptable:
        return STATUS_CODE_Not_Acceptable;

    case Request_URI_Too_long:
        return STATUS_CODE_Request_URI_Too_long;

    case Internal_Server_Error:
        return STATUS_CODE_Internal_Server_Error;

    default:
        return NULL;
    }
}

void print_usage(const char *PN){
    // "Specify document root <docroot> for built-in web server"
    fprintf(stderr, "usage: %s -p port -P password -S special_dir [-t docroot]\n", PN);
    exit(EXIT_FAILURE);
}

int port_parser(char *p){
    int i = strtol(p, NULL, 10);
    if (i != 0 && i > 0){
        if (i > 65535 || i < 0){
            fprintf(stderr, "NOT A VALID PORT!!!\n");
            exit(1);        
        }
        return i;
    }else{
        fprintf(stderr, "NOT A VALID PORT!!!\n");
        exit(1);
    }
}

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
	
	// char buf[1];
	// char tmp[BUFSIZE / 2];
	// size_t bytes_read;

	// bzero(&tmp, BUFSIZE / 2);
	// bzero(&buf, 1);

    // if(realpath(ERROR_PAGE, tmp) == NULL){
    // 	printf("ERROR(bad path): %s\n", buf);
    // 	return NULL;
    // }

    // FILE *file = fopen(tmp, "r");
    // // FILE *file = fopen(actualpath, "r");
    // if(file == NULL){
    // 	fprintf(stderr, "ERROR(open): %s\n", tmp);
    // 	return NULL;
    // }

	// bzero(&tmp, sizeof(tmp));
	
	// unsigned int size = 0;

    // int count = 0;
    // int count2 = 0;
    // int starting_pos[10];
    // short starting_pos_typ[10];
    // bool m = TRUE;
    // bool s = FALSE;

    // for (int i = 0; i < 10; ++i){
    // 	starting_pos[i] = -1;
    // 	starting_pos_typ[i] = -1;
    // }
    // char ERROR_PAGE_VAR[2][11] = {"ERROR_NAME\0", "ERROR_TEXT\0"};
    // while((bytes_read = fread(buf, 1, 1, file)) > 0){
	// 	if (ERROR_PAGE_VAR[0][0] == buf[0] || ERROR_PAGE_VAR[1][0] == buf[0])
	// 		s = TRUE;

    // 	if ((ERROR_PAGE_VAR[0][count] == buf[0] || ERROR_PAGE_VAR[1][count] == buf[0]) && s ){
    // 		count++;
    // 		m = TRUE;
    // 	}else
    // 		m = FALSE;

    // 	// if (m) strncat(tmp, buf, sizeof(buf));
    	
    // 	// printf("%d", strlen(tmp));
    // 	if (count == 10 && strlen(tmp) == 10 && count2 < 10) {
    // 		starting_pos[count2] = ftell(file) - 10;
    		
    // 		starting_pos_typ[count2] = (strncmp(ERROR_PAGE_VAR[0], tmp)) ? 0 : 1;

    // 		bzero(&tmp, sizeof(tmp));
    // 		count = 0;
    // 		count2++;
    // 		m = FALSE;
    // 	}
    // }

    // rewind(file);
    // count = 0;
    // count2 = 0;
    // while((bytes_read = fread(buf, 1, 1, file)) > 0){
    // 	size += bytes_read;
    // 	for (int i = 0; i < 10; ++i){
    // 		if (starting_pos[i] == -1) break;
    // 		if (starting_pos_typ[i] == -1) break;
    // 		if (starting_pos[i] == (size - 1)) {
    // 			m = TRUE;
    // 			count2 = i;
    // 			break;
    // 		}
    // 	}
    // 	if (m && count == 0){
    // 		if (starting_pos_typ[count2]){
    // 			if (error_code >= 2 && error_code <= 4)
    // 				dprintf(client_socket, "%s <br>\"<b>%s</b>\"", ERROR_TYP_TXT[error_code], file_name);
    // 			else
    // 				dprintf(client_socket, "%s", ERROR_TYP_TXT[error_code]);
    // 		}
    // 		else
    // 			dprintf(client_socket, "%s", STATUS_CODE[error_code]);

    // 	}
 	// 	if (count == 10) {
 	// 		m = FALSE;
 	// 		count = 0;
 	// 	}
 	// 	if (m) count++;

    // 	if (!m){
    //         write(client_socket, buf, sizeof(buf));
    //     }
    // }
    // dprintf(client_socket, "%s", EOH);
    // fclose(file);
    return NULL;
}
