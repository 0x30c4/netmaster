#include "handler.h"

const char VALID_HEADERS_FROM_SLAVE[5][15] = {"GET\0", "POST\0", "Cookie\0", "Content-Length\0", "Content-Type\0"};

const char FILE_EXT[9][6] = {"html\0", "htm\0", "js\0", "css\0", "txt\0",
                            "png\0", "jpg\0", "jpge\0", "ico\0"};

char *PARSED_FROM_HEADERS[3] = {"\0", "\0", "\0"}; //file, Cookie, GET/POST data, content length.
char SPECIAL_DIR[BUFSIZE / 4] = "\0";
char PASSWORD[BUFSIZE / 128] = "\0";

// for bit masking the values 

int CONTENT_LEN = -1;
unsigned short GET_R   = 0x01;
unsigned short POST_R  = 0x02;
unsigned short CON_LEN = 0x04;
unsigned short CON_TYP = 0x08;

/*
    checks if the file is accessible by the 
    server starter user and not a directory/special file.
*/
int fileChecker(char fn[BUFSIZE / 4], unsigned long long int *size, short *file_type){

    // checking for path trivial attack.
    if(startsWith(fn, "/.") || startsWith(fn, ".") || 
            startsWith(fn, "//")){
        printf("attack!!\n");
        return Not_Acceptable;
    }
    // checking if the requested recourse is in the special directory.  
    if (startsWith(realpath(fn, NULL), SPECIAL_DIR))
        return Forbidden;

    struct stat sb;
    if( access( fn, F_OK ) == -1 ) return Not_Found;

    if (stat(fn, &sb) == -1)
        return Forbidden;

    // checking if the file is owned by the user who started the server.
    if (getuid() != sb.st_uid) return Forbidden;

    // checking for other errors.
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  return Forbidden; break;
        case S_IFCHR:  return Forbidden; break;
        case S_IFDIR:  return Forbidden; break;
        case S_IFIFO:  return Forbidden; break;
        case S_IFLNK:  return Forbidden; break;
        case S_IFREG:                    break;
        case S_IFSOCK: return Forbidden; break;
        default:       return Forbidden; break;
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
    //checking the file permission.
    // printf("%lo %lo %lo\n", (unsigned long)(sb.st_mode & S_IRUSR), (unsigned long)sb.st_mode, (unsigned long)S_IRUSR);
    if (sb.st_mode & S_IRUSR)
        return OK;
    else
        return Forbidden;
}

// parsing, what kind of request is made by the slave
void httpRequestHandler(int client_socket, int server_socket){
    int size;
    unsigned long long file_size = 0;
    unsigned short is_get_post_con_type_len = 0x0;
    short ret_code = -1;
    short file_type = -1;
    char buf[BUFSIZE];
    short eohlCounter = 0;
    bool loop = TRUE;

    while(loop){
       /* 
            the readLine() function read any thing from a FD 
            until it hits a "\n".
            this function was inspired by the python's readlines method. 
       */
        if((size = readLine(client_socket, buf, BUFSIZE)) == 0) break;
        
        /*
            every time the "buf" variable is equal to "\r\n"
            eohlCounter value is incised by one.
            stringcmp() is a costume function for comparing 
            two strings.
        */
        if (stringcmp(buf, EOHL)) eohlCounter += 1;

        if (eohlCounter == 1 && (is_get_post_con_type_len & GET_R) == GET_R)
            loop = FALSE;


        // printf("%d | %d | %s", CONTENT_LEN, size, buf);
        if ((is_get_post_con_type_len & POST_R) == POST_R && eohlCounter == 1){
            char c[1] = "\0";
            size = 0;
            bzero(buf, BUFSIZE);
            while(TRUE){
                if (read(client_socket, &c, 1) == 1){
                    buf[size] = c[0];
                    size++;
                }
                else
                    break;
                if (size == CONTENT_LEN) break;
            }
            if (size > CONTENT_LEN || size < CONTENT_LEN)
                ret_code = Bad_Request;
            if (size > (BUFSIZE / 128) + 5)
                ret_code = Unauthorized;
            PARSED_FROM_HEADERS[POST_D] = strndup(buf, strlen(buf));
            break;
        }

        /*
            parses http request from the "buf"
        */
        if (ret_code == -1 && !((is_get_post_con_type_len & POST_R) == POST_R && eohlCounter == 1))
            httpReqestParser(buf, &is_get_post_con_type_len, &ret_code, &eohlCounter, &loop);
    }
    // client_socket = 1;
    // headerSender(client_socket, HTML, 401, OK);
    // fileSender(client_socket, INDEX_FILE);

    // checking if the request doesn't end with a EOH. 
    if (eohlCounter == 0)
        ret_code = Bad_Request;

    if (ret_code == -1)
        ret_code = requestValidity(&is_get_post_con_type_len);

    /*
        checking if the posted data is valid or not.
        if it's valid then it checks the pass code.
    */
    if ((ret_code == OK || ret_code == -1) &&
        (is_get_post_con_type_len & POST_R) == POST_R)
        ret_code = postDataHandler(PARSED_FROM_HEADERS[POST_D]);

    // if the return code is not changed then the file checker checks the file.
    if (ret_code == OK)
        ret_code = fileChecker(PARSED_FROM_HEADERS[FILE_NAME], &file_size, &file_type);

    // sending the header
    headerSender(client_socket, file_type, file_size, ret_code);

    if (ret_code == OK) fileSender(client_socket, PARSED_FROM_HEADERS[FILE_NAME]);
    else errorPageSender(client_socket, ret_code, PARSED_FROM_HEADERS[FILE_NAME]);

    // freeing the allocated memory occupied by PARSED_FROM_HEADERS array    
    for (int i = 0; i < 3; ++i){
        if (PARSED_FROM_HEADERS[i] != "\0")
            PARSED_FROM_HEADERS[i] = "\0";
    }
    fflush(stdout);
    shutdown(client_socket, SHUT_RD);
    close(client_socket);
}

void *httpReqestParser(char *header, unsigned short *is_get_post_con_type_len, short *ret_code, short *eohlCounter, bool *loop){
    if((!endsWith(header, EOHL) && (*is_get_post_con_type_len & POST_R) == POST_R) &&
        *eohlCounter != 1) {
        printf("Bad_Request 3 | %s\n", header);
        *loop = FALSE;
        *ret_code = Bad_Request;
        return NULL;
    }
    int len_split = 0;

    len_split = 0;
    char **splited_header = split(header, " ", &len_split);

    /* 
        checking for valid headers. and the context also.
        because i didn't made this server to handle extra data 
        from get request.so, content-length and 
        content-type header is not something that i need.
        for that reason whenever a get request contents those headers
        server sends not acceptable error.
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
    // printf("CON_TYP : %d\n", (*is_get_post_con_type_len & CON_TYP) == CON_TYP);
    // printf("CON_LEN : %d\n", (*is_get_post_con_type_len & CON_LEN) == CON_LEN);

    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[GET]) || 
        startsWith(header, VALID_HEADERS_FROM_SLAVE[POST])){
        if (len_split != 3 || strncmp(splited_header[2], "HTTP/1.1\r\n", 10)) {
            printf("Bad_Request 1\n");
            *loop = FALSE;
            *ret_code = Bad_Request;
            return NULL;
        }
    }else if(startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTLENGTH]) ||
                startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTTYPE])){
        if(len_split != 2){
            printf("Bad_Request 2\n");
            *loop = FALSE;
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
                    printf("Not_Acceptable 3\n");
                    *loop = FALSE;
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
                if (strlen(data) > (BUFSIZE / 4)){
                    *loop = FALSE;
                    *ret_code = Not_Acceptable;
                    return NULL;
                }
                PARSED_FROM_HEADERS[FILE_NAME] = strndup(data, strlen(data));
            }else {
                PARSED_FROM_HEADERS[FILE_NAME] = INDEX_FILE;
            }
            free(data);
            return NULL;
        }
    }
    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[COOKIE])){
        if (strlen(header) > (BUFSIZE / 4)){
            *ret_code = Not_Acceptable;
            *loop = FALSE;
            return NULL;
        }
        PARSED_FROM_HEADERS[COOKIE_D] = strndup(header, strlen(header));
        return NULL;
    }

    if (startsWith(header, VALID_HEADERS_FROM_SLAVE[CONTENTLENGTH]) && (*is_get_post_con_type_len & POST_R) == POST_R){
        if (strlen(header) > (BUFSIZE / 128)){
            *ret_code = Not_Acceptable;
            *loop = FALSE;
            return NULL;
        }
        CONTENT_LEN = contentLength(header);
        return NULL;
    }
    return NULL;
}

int postDataHandler(char buf[(BUFSIZE / 128) + 5]){
    int buf_len = strlen(buf);
    // if (buf_len != CONTENT_LEN) return Bad_Request;
    if (!startsWith(buf, "pass=")){
        printf("Not_Acceptable 66 | %s\n", buf);
        return Not_Acceptable;
    }
    char pass[BUFSIZE / 128];
    bzero(&pass, BUFSIZE / 128);
    slice_str(buf, pass, 5, buf_len);
    printf("%s | %s", pass, PASSWORD);
    if (stringcmp(pass, PASSWORD))
        return OK;
    else
        return Unauthorized;
    // fprintf(stderr, "%s\n", pass);
}

int requestValidity(unsigned short *is_get_post_con_type_len){
    //checking if the get request have any none necessary headers. 
    if ((*is_get_post_con_type_len & GET_R) == GET_R &&
        ((*is_get_post_con_type_len & CON_LEN) == CON_LEN
        || (*is_get_post_con_type_len & CON_TYP) == CON_TYP)) {        
        printf("Not_Acceptable 1\n");
        return Not_Acceptable;
    }

    // checking if the post request have required headers.  
    if ((*is_get_post_con_type_len & POST_R) == POST_R &&
        ((*is_get_post_con_type_len & CON_LEN) != CON_LEN ||
                 (*is_get_post_con_type_len & CON_TYP) != CON_TYP)) {
        printf("Not_Acceptable 2\n");
        return Not_Acceptable;
    }
    return OK;
}

void * handle_connection(void* args){
    // handling the slaves.
    // int client_socket = *((int*)args);
    // free(args);

    struct SERVER_CLIENT_DATA *server_client_fd = (struct SERVER_CLIENT_DATA*)args;
    // this function will parse the request from the slave.
    strncpy(SPECIAL_DIR, server_client_fd->special_dir, strlen(server_client_fd->special_dir));
    strncpy(PASSWORD, server_client_fd->password, strlen(server_client_fd->password));
    httpRequestHandler(server_client_fd->client, server_client_fd->server);

    return NULL;
}