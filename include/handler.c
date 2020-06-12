#include "handler.h"

char *PARSED_FROM_HEADERS[3]; //file, Cookie, GET/POST data, content length.
const char VALID_HEADERS_FROM_SLAVE[5][15] = {"GET\0", "POST\0", "Cookie\0", "Content-Length\0", "Content-Type\0"};

const char FILE_EXT[9][6] = {"html\0", "htm\0", "js\0", "css\0", "txt\0",
                            "png\0", "jpg\0", "jpge\0", "ico\0"};

// for bit masking the values 

unsigned short GET_R   = 0x01;
unsigned short POST_R  = 0x02;
unsigned short CON_LEN = 0x04;
unsigned short CON_TYP = 0x08;

/*
    checks if the file is accessible by the 
    server starter user and not a directory/special file.
*/
int fileChecker(char *fn, unsigned long long int *size, int *file_type){

    // checking for path trivial attack.
    if(startsWith(fn, "/.") || startsWith(fn, ".") || 
            startsWith(fn, "//")){
        printf("attack!!\n");
        return Not_Acceptable;
    }

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
        case S_IFDIR:  return Bad_Request  ; break;
        case S_IFIFO:  return ERRNOTREGFILE; break;
        case S_IFLNK:  return ERRNOTREGFILE; break;
        case S_IFREG:                        break;
        case S_IFSOCK: return ERRNOTREGFILE; break;
        default:       return ERRNOTREGFILE; break;
    }
    *size = sb.st_size;
    for (int i = 0; i < 9; ++i){
        // printf("%s || %s || %d\n", fn, FILE_EXT[i], endsWith(fn, FILE_EXT[i]));
        if (endsWith(strlow(strndup(fn, strlen(fn))), FILE_EXT[i])) {
            *file_type = i;
            break;
        }
    }
    if (*file_type == -1) *file_type = OCTET_STREAM;
    // printf("%d\n", *file_type);
    //checking the file permission.
    if (sb.st_mode & S_IRUSR){
        // strncpy(file, fn, strlen(fn));
        return OK;
    }else{
        return Unauthorized;
    }
}

// parsing, what kind of request is made by the slave
void requestType(int client_socket, int server_socket){
    unsigned long long file_size = 0;
    unsigned short is_get_post_con_type_len = 0x0;
    int ret_code = -1;
    int file_type = -1;
	char buf[BUFSIZE];
    short count = 0;

    while(TRUE){
        if(readLine(client_socket, buf) == 0) {
            if (count == 0) ret_code = Bad_Request;
            break;
        }

        if (ret_code == -1)
            headerParser(buf, &is_get_post_con_type_len, &ret_code);

        printf("%d || %s", strlen(buf), buf);
        if(stringcmp(buf, EOHL)){
            if ((is_get_post_con_type_len & POST_R) == POST_R && count != 0)
                break;
            else if ((is_get_post_con_type_len & GET_R) == GET_R)
                break;
            // else if ((is_get_post_con_type_len & POST_R) == POST_R && count == 0)
            count++;
        } 
    }

    if (ret_code == -1)
        ret_code = fileChecker(PARSED_FROM_HEADERS[FILE_NAME], &file_size, &file_type);    

    headerSender(client_socket, file_type, file_size, ret_code);

    if (ret_code == OK) fileSender(client_socket, PARSED_FROM_HEADERS[FILE_NAME]);
    
    // printf("%s\n", PARSED_FROM_HEADERS[0]);

    // it's here for testing and debugging.
    // close(client_socket);
    // close(server_socket);
    // exit(1);

  
    fflush(stdout);
    close(client_socket);
}   

void *httpHandler(char *header, unsigned short *is_get_post_con_type_len, int *ret_code){
    if(!endsWith(header, EOHL)) {
        printf("Bad_Request 3\n");
        *ret_code = Bad_Request;
        return NULL;
    }

    int len_split = 0;

    len_split = 0;
    char **splited_header = split(header, " ", &len_split);

    /* 
        checking for valid headers. and the context also.
        because when the a GET request can't handle content-length and 
        content-type header.i'm doing this because in my context 
        this is useless and i don't need this. 
    */

    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[GET])) 
        *is_get_post_con_type_len = (GET_R | *is_get_post_con_type_len);

    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[POST])) 
        *is_get_post_con_type_len = (POST_R | *is_get_post_con_type_len);
    
    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTLENGTH])) 
        *is_get_post_con_type_len = (CON_LEN | *is_get_post_con_type_len);
    
    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTTYPE])) 
        *is_get_post_con_type_len = (CON_TYP | *is_get_post_con_type_len);

    // printf("GET_R   : %d\n", *is_get_post_con_type_len & GET_R);
    // printf("POST_R  : %d\n", *is_get_post_con_type_len & POST_R);
    // printf("CON_TYP : %d\n", *is_get_post_con_type_len & CON_TYP);
    // printf("CON_LEN : %d\n", *is_get_post_con_type_len & CON_LEN);

    //checking if the get request have any none necessary headers. 
    if ((*is_get_post_con_type_len & GET_R) == GET_R &&
        ((*is_get_post_con_type_len & CON_LEN) == CON_LEN
        || (*is_get_post_con_type_len & CON_TYP) == CON_TYP)) {        
        *ret_code = Not_Acceptable;
        return NULL;
    }

    // checking if the post request have required headers.  
    if ((*is_get_post_con_type_len & POST_R) == GET_R &&
        (*is_get_post_con_type_len & CON_LEN) != CON_LEN &&
        (*is_get_post_con_type_len & CON_TYP) != CON_TYP) {
        *ret_code = Not_Acceptable;
        return NULL;
    }

    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[GET]) || 
        startsWith(header, VALID_HEADERS_FROM_SLAVE[POST])){
        if (len_split != 3 || strncmp(splited_header[2], "HTTP/1.1\r\n", 10)) {
            printf("Bad_Request 1\n");
            *ret_code = Bad_Request;
            return NULL;
        }
    }else if(startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTLENGTH]) ||
                startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTTYPE])){
        if(len_split != 2){
            printf("Bad_Request 2\n");
            *ret_code = Bad_Request;
            return NULL;
        }
    }

    char *data = malloc(sizeof(char*));
    bzero(data, sizeof(data)); 
    for (int i = 0; i < len_split; ++i){
        if (startsWith(splited_header[i], "/")) {
            int count = 0;
            int _len = strlen(splited_header[i]);
            for (; count < _len; ++count){
                if(splited_header[i][count] == '?'){ //|| splited_header[i][count] == '/'){
                    *ret_code = Not_Acceptable;
                    return NULL;
                }
            }
            /* 
                when the client request for the "/" then the fn length is zero.
                so, the fn is initialize with the index.html file. 
                returning the file name that client requested with get or post.
            */
            if (_len > 1){
                slice_str(splited_header[i], data, 1, _len);
                PARSED_FROM_HEADERS[FILE_NAME] = data;
            }else {
                PARSED_FROM_HEADERS[FILE_NAME] = INDEX_FILE;
                free(data);
            }
            printf("%s\n", PARSED_FROM_HEADERS[FILE_NAME]);
        }
    }
    if (startsWith(header, "Cookie:")){
        // printf("%d\n", startsWith(header, "Cookie:"));
        PARSED_FROM_HEADERS[COOKIE_D] = "NULL";
    }
    // printf("%s\n", PARSED_FROM_HEADERS[FILE_NAME]);
    // return data;
    return NULL;
}

void headerParser(char *header, unsigned short *is_get_post_con_type_len, int *ret_code){
    int len_split = 0;
    /*
        checking if the header is in the VALID_HEADERS_FROM_SLAVE array.
        if not then "NULL" will be returned.           
    */
    for (int i = 0; i < 5; ++i){
        if(!startsWith(header, VALID_HEADERS_FROM_SLAVE[i])){
            len_split++;
        }
    }

    if (len_split != 4) {
        // printf("%s", header);
        // return "NULL\0";

    }    
    httpHandler(header, is_get_post_con_type_len, ret_code);
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
	// headerSender(client_socket, HTML);

	// //sending file based on a request.
	fileSender(client_socket, INDEX_FILE);
}

void postHandler(int client_socket, const char *buf, int flag){
	// parsing the first line of client request.
	// printf("%s\n", buf);
	// headerParser(buf, _POST);

	//sending headers.
	// headerSender(client_socket, HTML);

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