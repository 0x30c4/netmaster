#include "sender.h"

// Server respones headers.
const char RESPONSE_GET_POST[17] = "HTTP/1.1 200 OK\r\n";
const char SERVER_DETAILS[24] = "Server: NETMASTER/0.69\r\n";
const char SERVER_CONTENT_TYPE[40] = "Content-type: text/html; charset=UTF-8\r\n";
const char CLOSE_GET_POST[21] = "Connection: close\r\n\r\n";

const char FAVICON_HEADER[28] = "Content-Type: image/x-icon\r\n";
const char FAVICON_CONTLEN[22] = "Content-Length: 8894\r\n";
const char WWW_AUTHENTICATE[88] = "WWW-Authenticate: Basic realm=UVdFUlRZVUlLU0RGR0hKSzwgVkZUWVVJS01OQlZGWVVLTDxNTkJWQwo=\r\n";

void headerSender(int client_socket, int flag){
	write(client_socket, RESPONSE_GET_POST, sizeof(RESPONSE_GET_POST));
	// write(1, RESPONSE_GET_POST, sizeof(RESPONSE_GET_POST));
	
	write(client_socket, SERVER_DETAILS, sizeof(SERVER_DETAILS));
	// write(1, SERVER_DETAILS, sizeof(SERVER_DETAILS));
	
	if (flag == IMAGE_ICO){
		write(client_socket, FAVICON_HEADER, sizeof(FAVICON_HEADER));		
		write(client_socket, FAVICON_CONTLEN, sizeof(FAVICON_CONTLEN));		
	}else if (flag == HTML){
		write(client_socket, SERVER_CONTENT_TYPE, sizeof(SERVER_CONTENT_TYPE));
		// write(1, SERVER_CONTENT_TYPE, sizeof(SERVER_CONTENT_TYPE));
	}

	write(client_socket, CLOSE_GET_POST, sizeof(CLOSE_GET_POST));
   	// write(client_socket, "Set-Cookie: _ga=asdayFTYFYJF>Mjasdj", 36);

}

void *fileSender(int client_socket, const char *filename){
	char buf[BUFSIZE];
	char actualpath[100];
	size_t bytes_read;

	bzero(&actualpath, sizeof(actualpath));
	bzero(&buf, sizeof(buf));

    if(realpath(filename, actualpath) == NULL){
    	printf("ERROR(bad path): %s\n", buf);
    	return NULL;
    }

    FILE *file = fopen(actualpath, "r");
    // FILE *file = fopen(actualpath, "r");
    if(file == NULL){
    	fprintf(stderr, "ERROR(open): %s\n", actualpath);
    	return NULL;
    }

    while((bytes_read = fread(buf, 1, BUFSIZE, file)) > 0){
    	// printf("Sending %zu bytes\n", bytes_read);
    	write(client_socket, buf, bytes_read);
    	// write(1, buf, bytes_read);
    }
    
    fclose(file);
    // printf("Closing connection.\n");
    return NULL;
}