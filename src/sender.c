#include "../include/sender.h"

// Server respones headers.

const char RESPONSE_HTTP[8] = "HTTP/2 \0";
const char SERVER_DETAILS[25] = "Server: NETMASTER/0.69\0";
// image/x-icon\r\n"; //application/octet-stream // text/x-c; charset=UTF-8 // image/png
const char CONTENT_TYPE[15] = "Content-Type: \0";
const char CONTENT_LENGTH[17] = "Content-Length: \0";
const char CLOSE_GET_POST[18] = "Connection: close\0";
                              // 0          0                            0                                    
const char FILE_EXT[9][6] = {".html\0", ".htm\0", ".js\0", ".css\0", ".txt\0", ".png\0", ".jpg\0", ".jpge\0", ".ico\0"};
                                
char CONT_TYPE_HTML[CONT_TYP_LEN_HTML + 1] = "text/html; charset=utf-8\0";
char CONT_TYPE_JS[CONT_TYP_LEN_JS + 1] = "application/javascript\0";
char CONT_TYPE_CSS[CONT_TYP_LEN_CSS + 1] = "text/css; charset=utf-8\0";
char CONT_TYPE_TXT[CONT_TYP_LEN_TXT + 1] = "text/plain; charset=utf-8\0";
char CONT_TYPE_PNG[CONT_TYP_LEN_PNG + 1] = "image/png\0";
char CONT_TYPE_JPG[CONT_TYP_LEN_JPG + 1] = "image/jpg\0";
char CONT_TYPE_JPGE[CONT_TYP_LEN_JPGE + 1] = "image/jpge\0";
char CONT_TYPE_ICON[CONT_TYP_LEN_ICON + 1] = "image/x-icon\0";
char CONT_TYPE_APP_OCT_STR[CONT_TYP_LEN_APP_OCT_STR + 1] = "application/octet-stream\0";

int getFileType(char * file_name){
    for (int i = 0; i <= OCTET_STREAM; i++){
        if (endsWith(file_name, FILE_EXT[i]))
            return i;
    }
    return 9;
}

void buildContTypeHeader(int index, int client_socket){
    write(client_socket, CONTENT_TYPE, 14);
    switch (index){

    case HTML:
        write(client_socket, CONT_TYPE_HTML, CONT_TYP_LEN_HTML);
        write(client_socket, EOHL, 2);
        break;

    case HTM:
        write(client_socket, CONT_TYPE_HTML, CONT_TYP_LEN_HTML);
        write(client_socket, EOHL, 2);
        break;

    case JS:
        write(client_socket, CONT_TYPE_JS, CONT_TYP_LEN_JS);
        write(client_socket, EOHL, 2);
        break;

    case CSS:
        write(client_socket, CONT_TYPE_CSS, CONT_TYP_LEN_CSS);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_PNG:
        write(client_socket, CONT_TYPE_PNG, CONT_TYP_LEN_PNG);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_JPG:
        write(client_socket, CONT_TYPE_JPG, CONT_TYP_LEN_JPG);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_JPGE:
        write(client_socket, CONT_TYPE_JPGE, CONT_TYP_LEN_JPGE);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_ICO:
        write(client_socket, CONT_TYPE_ICON, CONT_TYP_LEN_ICON);
        write(client_socket, EOHL, 2);
        break;
    
    case TEXT:
        write(client_socket, CONT_TYPE_TXT, CONT_TYP_LEN_TXT);
        write(client_socket, EOHL, 2);
        break;

    case OCTET_STREAM:
        write(client_socket, CONT_TYPE_APP_OCT_STR, CONT_TYP_LEN_APP_OCT_STR);
        write(client_socket, EOHL, 2);
        break;
    }
}

void sendResponse(int client_socket, SERVER_ROOT * server_data, char * file_name){
    char *full_path = calloc(server_data->len + MAX_HEADER, 1);

    long int fsize = -1;

    int retcode = PathChecker(file_name, full_path, &fsize, server_data);

    // printf("%s\n", full_path);
    // printf("--> %s\n", CONT_TYP[getFileType(full_path)]);
    // printf("----> %ld\n", fsize);

    headerSender(client_socket, full_path, fsize, retcode);

    if (retcode == OK)
        fileSender(client_socket, full_path);
    else
        sendErrPage(client_socket, retcode);

    free(full_path);
}

void headerSender(int client_socket, char * file_name, long int fsize, int status_code){
    
    write(client_socket, RESPONSE_HTTP, 7);
    write(client_socket, HeaderErrNoStatusCode(status_code), ERR_STATUS_CODE_LEN[status_code]);
    write(client_socket, EOHL, 2);

    if (status_code == OK){
        buildContTypeHeader(getFileType(file_name), client_socket);
        // write(client_socket, CONTENT_LENGTH, 17);
        // write(client_socket, &fsize, sizeof(fsize));
        // write(client_socket, EOHL, 2);
    }else {
        buildContTypeHeader(HTML, client_socket);
    }
    
    write(client_socket, SERVER_DETAILS, 22);
    write(client_socket, EOHL, 2);

    write(client_socket, CLOSE_GET_POST, 17);
    write(client_socket, EOH, 4);
    
}

void sendErrPage(int client_socket, int retcode){
    printf("ERR\n");
}

void *fileSender(int client_socket, const char *filename){
    int file_fd = open(filename, O_RDONLY);
    char c;
    while ((read(file_fd, &c, 1)) > 0)
        write(client_socket, &c, 1);

    close(file_fd);
    return NULL;
}
