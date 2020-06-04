#include "handler.h"

char *PARSED_FROM_HEADERS[4]; //file, Cookie, GET/POST data, content length.
char VALID_HEADERS_FROM_SLAVE[5][15] = {"GET", "POST", "Cookie", "Content-Length", "Content-Type"};

/*
    checks if the file is accessible by the 
    server starter user and not a directory/special file.
*/
int fileChecker(char *fn, unsigned long long int *size){
    struct stat sb;

    if( access( fn, F_OK ) == -1 ) return Not_Found;

    if (stat(fn, &sb) == -1) {
        return Forbidden;
    }
    // checking if the file is owned by the user who started the server.
    if (getuid() != sb.st_uid) return Unauthorized;

    // checking for other errors.
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  return ERRNOTREGFILE; break;
        case S_IFCHR:  return ERRNOTREGFILE; break;
        case S_IFDIR:  return Bad_Request; break;
        case S_IFIFO:  return ERRNOTREGFILE; break;
        case S_IFLNK:  return ERRNOTREGFILE; break;
        case S_IFREG:                        break;
        case S_IFSOCK: return ERRNOTREGFILE; break;
        default:       return ERRNOTREGFILE; break;
    }
    *size = sb.st_size;
    //checking the file permission.
    return (sb.st_mode & S_IRUSR) ? OK : Unauthorized;
}

// parsing, what kind of request is made by the slave
void requestType(int client_socket, int server_socket){
	char buf[BUFSIZE];
	char *buf2;
    char file[BUFSIZE / 2];
    short flag = 0;
	short flag_2 = 0;
    unsigned long long file_size = 0;
    bzero(&file, sizeof(file));
    while(TRUE){
        readLine(client_socket, buf);
        buf2 = headerParser(buf, &flag);
            
        printf("%s", buf);
        if(buf2 != NULL) {
            printf("%s\n", buf2);
        	if (flag == FILE_NAME){
                printf("%d", fileChecker(buf2, &file_size));
                if (fileChecker(buf2, &file_size) == OK) strncpy(file, buf2, sizeof(buf2));
            }
        	if (flag == COOKIE){
                printf("COOKIE: %s\n", buf2);    
            }
        }
        // if(endsWith(buf, EOHL) && strlen(buf) > 2) printf("%s", buf);
        if(stringcmp(buf, EOHL)) break;
    }
    
    printf("FILE NAME: %s\n", file);

	headerSender(client_socket, HTML);
	fileSender(client_socket, INDEX_FILE);

    // it's here for testing and debugging.
    // close(client_socket);
    // close(server_socket);
    // exit(1);

  
    fflush(stdout);
    close(client_socket);
}


char *headerParser(char *header, short *flag){
    *flag = 0;
   	if(!endsWith(header, EOHL)) return "400"; 

    for (int i = 0; i < 5; ++i){
        if(!startsWith(header, VALID_HEADERS_FROM_SLAVE[i])) return NULL;
    }    

    int len_split = 0;

    char **splited_header = split(header, " ", &len_split);

    char *data = malloc(sizeof(char*));
    bzero(data, sizeof(data));
    
    for (int i = 0; i < len_split; ++i){
        if (startsWith(splited_header[i], "/") && 
         endsWith(header, "HTTP/1.1\r\n") && 
         strlen(splited_header[i]) > 2) {
            // checking for path trivial attack.
            if(startsWith(splited_header[i], "/.") ||
                startsWith(splited_header[i], "//")){
                printf("attack!!\n");
                return FALSE;
            }

            size_t _len = strlen(splited_header[i]);
            int count = 0;
            for (; count < _len; ++count){
                if(splited_header[i][count] == '?'){ //|| splited_header[i][count] == '/'){
                    _len = count - 1;
                    // printf("%d\n", count);
                    break;
                }
            }
            printf("%d\n", stringcmp(splited_header[i], "/"));
            if (stringcmp(splited_header[i], "/")) return INDEX_FILE;

            slice_str(splited_header[i], data, 1, _len);
            *flag = FILE_NAME;
            // returning the file name that client requested with get or post.
            return data;
        }
    }
    if (startsWith(header, "Cookie:")){
        *flag = COOKIE;
	    // printf("%d\n", startsWith(header, "Cookie:"));
        return cookieHandler(header);
    }
    // printf("%s\n", PARSED_FROM_HEADERS[FILE_NAME]);
    return data;
}

char *cookieHandler(char *cookie){
	// printf("%s\n", cookie);
	int cookie_v_size = 0;
	char **cookie_v = split(cookie, " ", &cookie_v_size);
	if (cookie_v_size != 2) return FALSE;
	if(startsWith(cookie_v[1], "_aed=") && strlen(cookie_v[1]) == 23){
		slice_str(cookie_v[1], cookie, 5, 23);
		for (int i = 0; i < 4; ++i){
		}
		// printf("%s\n", cookie);
	}
	return "NULL";
}


void * handle_connection(void* args){
	// handling the slaves.
	// int client_socket = *((int*)args);
	// free(args);

	struct SERVER_CLIENT_FDS *server_client_fd = (struct SERVER_CLIENT_FDS*)args;


	// this function will parse the request from the slave.
	// requestType(client_socket);
	requestType(server_client_fd->client, server_client_fd->server);

    close(server_client_fd->client);
    return NULL;
}


/*
	this function is only for handling the connection 
	that are made by the USER from the browser.
*/
void getHandler(int client_socket, const char *buf){
	char file[1024];
	bzero(&file, sizeof(file));
	// parsing the first line of client request.
	// headerParser(buf, GET, file);

    printf("%s\n", file);

	//sending headers.
	headerSender(client_socket, HTML);

	// //sending file based on a request.
	fileSender(client_socket, INDEX_FILE);
}

void postHandler(int client_socket, const char *buf, int flag){
	// parsing the first line of client request.
	// printf("%s\n", buf);
	// headerParser(buf, _POST);

	//sending headers.
	headerSender(client_socket, HTML);

	//sending file based on a request.
	fileSender(client_socket, INDEX_FILE);

 //    int split_len = 0;
 //    char **buf_split = split(buf, "\n", &split_len);

 //    for (int i = 0; i < split_len; ++i){
 //    	printf("%s ||", buf_split[i]);
 //    }

 //    free(buf_split);
	// fflush(stdout);
	// fileSender(client_socket, "slave.html");
	// headerSender(client_socket, flag);
	// Content-Length
}