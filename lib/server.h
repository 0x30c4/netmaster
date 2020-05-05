#include "common.h"
#include "file.h"
#include "err.h"

#define SERVERPORT 6969
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int setup_sever(short port, int backlog){
	int server_socket, client_socket, addr_size;
	SA_IN server_addr;

	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket.");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind Failed.");

	check(listen(server_socket, backlog), "Listen Failed.");

	return server_socket;
}


int accept_new_connection(int server_socket){
	int client_socket, addr_size = sizeof(SA_IN);
	SA_IN client_addr; 

	check((client_socket =
		accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)), 
		"Accept Failed.");

	return client_socket;
}


void * handel_connection(int client_socket){
	int c;
	char buf[BUFSIZE];
	// FILE* f = fdopen(client_socket, "r");

    bzero(&buf, sizeof(buf));
    
    read(client_socket, buf, BUFSIZE);

    printf("%s\n", buf);

	for (int i = 0; i < strlen(buf); ++i){
		if(buf[i] == '\n'){
			buf[i] = '\0';
		}
	}

    if (strcmp(buf, "exit") == 10){
    	printf("Closing..\n");
    	close(client_socket);
    	// fclose(f);
    	exit(0);    	
    }

    file_reader(buf, client_socket);

    close(client_socket);
}

