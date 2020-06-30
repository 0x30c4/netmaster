#include "sender.h"

// Server respones headers.
const char RESPONSE_GET_POST[10] = "HTTP/1.1 \0";

const char SERVER_DETAILS[25] = "Server: NETMASTER/0.69\0";
const char CLOSE_GET_POST[22] = "Connection: close\0";

const char CONTENT_TYPE[15] = "Content-Type: \0";// image/x-icon\r\n"; //application/octet-stream // text/x-c; charset=UTF-8 // image/png
const char CONTENT_LENGTH[17] = "Content-Length: \0";


// const char WWW_AUTHENTICATE[88] = "WWW-Authenticate: Basic realm=UVdFUlRZVUlLU0RGR0hKSzwgVkZUWVVJS01OQlZGWVVLTDxNTkJWQwo=\r\n";
// const char WWW_AUTHENTICATE2[88] = "Set-Cookie: Basic realm=UVdFUlRZVUlLU0RGR0hKSzwgVkZUWVVJS01OQlZGWVVLTDxNTkJWQwo=\r\n";

void headerSender(int client_socket, int flag, unsigned long long int size, int status_code){
	dprintf(client_socket, "%s%s%s%s%s", RESPONSE_GET_POST, STATUS_CODE[status_code], EOHL,
											SERVER_DETAILS, EOHL);

	if (TEXT >= flag || status_code != OK)
		dprintf(client_socket, "%stext/%s; charset=UTF-8%s", CONTENT_TYPE, 
			(status_code != OK) ? FILE_EXT[HTML] : FILE_EXT[flag], EOHL);
	else if (IMAGE_ICO >= flag)
		dprintf(client_socket, "%simage/x-%s%s%s", CONTENT_TYPE, FILE_EXT[flag], 
			(flag == 8) ? "n" : "", EOHL);
	else if (flag == OCTET_STREAM)
		dprintf(client_socket, "%sapplication/octet-stream%s", CONTENT_TYPE, EOHL);

    if (status_code == OK)
        dprintf(client_socket, "%s%lld%s", CONTENT_LENGTH, size, EOHL);
	// dprintf(client_socket, "%s%s", WWW_AUTHENTICATE2, EOHL);
	
    dprintf(client_socket, "%s%s", CLOSE_GET_POST, EOH);
}

void *fileSender(int client_socket, const char *filename){
	char buf[BUFSIZE];
	char actualpath[100];
	size_t bytes_read;

	bzero(&actualpath, sizeof(actualpath));
	bzero(&buf, sizeof(buf));

    if(realpath(filename, actualpath) == NULL){
    	fprintf(stderr, "ERROR(bad path): %s\n", buf);
    	return NULL;
    }

    FILE *file = fopen(actualpath, "r");
    // FILE *file = fopen(actualpath, "r");
    if(file == NULL){
    	fprintf(stderr, "ERROR(open): %s\n", actualpath);
    	return NULL;
    }
    unsigned long int size = 0;
    while((bytes_read = fread(buf, 1, BUFSIZE, file)) > 0){
    	size += bytes_read;
        // printf("Sending %zu bytes\n", bytes_read);
    	// write(client_socket, buf, bytes_read);
        dprintf(client_socket, "%s", buf);
    	// dprintf(2, "%s", buf);
    	// write(1, buf, bytes_read);
    }
    
    // printf("total sent %lu bytes\n", size);
    dprintf(client_socket, "%s", EOH);
    fclose(file);
    // printf("Closing connection.\n");
    return NULL;
}