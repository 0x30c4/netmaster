#include "../include/sender.h"

// Server respones headers.

const char RESPONSE_HTTP[7] = "HTTP/2 ";
const char SERVER_DETAILS[25] = "Server: NETMASTER/0.69\0";
// image/x-icon\r\n"; //application/octet-stream // text/x-c; charset=UTF-8 // image/png
const char CONTENT_TYPE[15] = "Content-Type: \0";
const char CONTENT_LENGTH[17] = "Content-Length: \0";
const char CLOSE_GET_POST[22] = "Connection: close\0";
                              // 0          0                            0                                    
const char FILE_EXT[9][6] = {".html\0", ".htm\0", ".js\0", ".css\0", ".txt\0", ".png\0", ".jpg\0", ".jpge\0", ".ico\0"};
const char CONT_TYP[12][25] = {"html; charset=utf-8\0", "htm\0", "application/javascript\0", "css\0", "text; charset=utf-8\0", "png\0", "jpg\0", "jpge\0", "x-icon\0", "application/octet-stream\0","image/\0", "text/\0"};
const int CONT_TYP_LEN[12] = {20, 4, 11, 4, 20, 4, 4, 5, 7, 25, 7};

int getFileType(char * file_name){
    for (int i = 0; i <= OCTET_STREAM; i++){
        if (endsWith(file_name, FILE_EXT[i]))
            return i;
    }
    return 9;
}

void buildContTypeHeader(int index, int client_socket){
    write(client_socket, CONTENT_TYPE, 15);
    switch (index){

    case HTML:
        write(client_socket, CONT_TYP[11], CONT_TYP_LEN[11]);
        write(client_socket, CONT_TYP[HTML], CONT_TYP_LEN[HTML]);
        write(client_socket, EOHL, 2);
        break;

    case HTM:
        write(client_socket, CONT_TYP[11], CONT_TYP_LEN[11]);
        write(client_socket, CONT_TYP[HTML], CONT_TYP_LEN[HTML]);
        write(client_socket, EOHL, 2);
        break;

    case JS:
        // printf("->>>> %s\n", CONT_TYP[JS]);
        // write(client_socket, CONT_TYP[JS],  CONT_TYP_LEN[JS]);
        write(client_socket, "application/javascript\0", 22);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_PNG:
        write(client_socket, CONT_TYP[10], CONT_TYP_LEN[10]);
        write(client_socket, CONT_TYP[IMAGE_PNG], CONT_TYP_LEN[IMAGE_PNG]);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_JPG:
        write(client_socket, CONT_TYP[10], CONT_TYP_LEN[10]);
        write(client_socket, CONT_TYP[IMAGE_JPG], CONT_TYP_LEN[IMAGE_JPG]);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_JPGE:
        write(client_socket, CONT_TYP[10], CONT_TYP_LEN[10]);
        write(client_socket, CONT_TYP[IMAGE_JPGE], CONT_TYP_LEN[IMAGE_JPGE]);
        write(client_socket, EOHL, 2);
        break;

    case IMAGE_ICO:
        write(client_socket, CONT_TYP[10], CONT_TYP_LEN[10]);
        write(client_socket, CONT_TYP[IMAGE_ICO], CONT_TYP_LEN[IMAGE_ICO]);
        write(client_socket, EOHL, 2);
        break;
    
    case TEXT:
        // write(client_socket, CONT_TYP[12], CONT_TYP_LEN[12]);
        write(client_socket, "text/\0", 6);
        write(client_socket, CONT_TYP[TEXT], CONT_TYP_LEN[TEXT]);
        write(client_socket, EOHL, 2);
        break;

    case OCTET_STREAM:
        write(client_socket, CONT_TYP[10], CONT_TYP_LEN[10]);
        write(client_socket, CONT_TYP[IMAGE_ICO], CONT_TYP_LEN[IMAGE_ICO]);
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

    fileSender(client_socket, full_path);

    free(full_path);
}

void headerSender(int client_socket, char * file_name, long int fsize, int status_code){
    
    write(client_socket, RESPONSE_HTTP, 7);

    if (status_code == OK){
        write(client_socket, HeaderErrNoStatusCode(status_code), OK_Len);
        write(client_socket, EOHL, 2);
        buildContTypeHeader(getFileType(file_name), 1);

        // write(client_socket, CONTENT_LENGTH, 17);
        // write(client_socket, &fsize, sizeof(fsize));
        // write(client_socket, EOHL, 2);
        
        write(client_socket, SERVER_DETAILS, 25);
        write(client_socket, EOHL, 2);

        write(client_socket, CLOSE_GET_POST, 22);
    }
    
    write(client_socket, EOH, 4);
    
}

void *fileSender(int client_socket, const char *filename){
    int file_fd = open(filename, O_RDONLY);

    // printf("223\n");
    char c;
    // printf("%d | %d\n", read(file_fd, &c, 1), file_fd);
    while ((read(file_fd, &c, 1)) > 0)
        write(client_socket, &c, 1);
    
    // write(client_socket, &"a", 1);

    close(file_fd);
    return NULL;
}
