#include "../include/sender.h"

// Server respones headers.

const char RESPONSE_GET_POST[10] = "HTTP/1.1 \0";
const char SERVER_DETAILS[25] = "Server: NETMASTER/0.69\0";
const char CONTENT_TYPE[15] = "Content-Type: \0";// image/x-icon\r\n"; //application/octet-stream // text/x-c; charset=UTF-8 // image/png
const char CONTENT_LENGTH[17] = "Content-Length: \0";
const char CLOSE_GET_POST[22] = "Connection: close\0";

void sendResponse(int client_socket, SERVER_ROOT * server_data, char * file_name){
    char *full_path = calloc(server_data->len + MAX_HEADER, 1);
    
    long int fsize = -1;

    int retcode = PathChecker(file_name, full_path, &fsize, server_data);

    printf("----> %ld\n", fsize);

    free(full_path);
}

void headerSender(int client_socket, long int fsize, int status_code){
    
}

void *fileSender(int client_socket, const char *filename){
    return NULL;
}
