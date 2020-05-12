#include "server.h"

int setup_sever(short port, int backlog){
	int server_socket;
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
	char client_addr_str[32];

	printf("Listenig on port %d\n", SERVERPORT);

	check((client_socket =
		accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)), 
		"Accept Failed.");

	inet_ntop(AF_INET, &client_addr, client_addr_str, 100);

	printf("\033[01;32m[+]\033 A new slave is connected %s\n", client_addr_str);

	return client_socket;
}


void * handel_connection(void* pclient_scoket){
	int client_socket = *((int*)pclient_scoket);

	free(pclient_scoket);

	//test
	// struct SERVER_CLIENT_FDS *scd= (struct SERVER_CLIENT_FDS*)server_client_fd;

	// server_socket = scd->server;
	// client_socket = scd->client;

	char buf[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
	char actualpath[100];
    bzero(&buf, sizeof(buf));
    
    while((bytes_read = read(client_socket, buf+msgsize, sizeof(buf)-msgsize)))	{
    	msgsize += bytes_read;
    	if(msgsize > BUFSIZE-1 || buf[msgsize-1] == '\n') break;
    }

    check(bytes_read, "read error");
    buf[msgsize-1] = 0;

    printf("REQUEST: %s\n", buf);
    fflush(stdout);
    
    if(realpath(buf, actualpath) == NULL){
    	printf("ERROR(bad path): %s\n", buf);
    	close(client_socket);
    	return NULL;
    }

    FILE *fp = fopen(actualpath, "r");
    if(fp == NULL){
    	printf("ERROR(open): %s\n", buf);
    	close(client_socket);
    	return NULL;
    }

    while((bytes_read = fread(buf, 1, BUFSIZE, fp)) > 0){
    	printf("Sending %zu bytes\n", bytes_read);
    	write(client_socket, buf, bytes_read);
    }
    
    write(client_socket, "\n", 1);

    close(client_socket);
    fclose(fp);
    printf("Closing connection.\n");
    return NULL;
}

// void with_select(int server_socket){

// 	fd_set current_sockets, ready_sockets;

// 	FD_ZERO(&current_sockets);
// 	FD_SET(server_socket, &current_sockets);

// 	while(1){
// 		ready_sockets = current_sockets;

// 		check(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "select error.");

// 		for (int i = 0; i < FD_SETSIZE; ++i){
// 			if(FD_ISSET(i, &ready_sockets)){
// 				if(i == server_socket){
// 					int client_socket = accept_new_connection(server_socket);
// 					FD_SET(client_socket, &current_sockets);
// 				}else{
// 					handel_connection(i, server_socket);
// 					FD_CLR(i, &current_sockets);
// 				}
// 			}
// 		}
// 		// int client_socket = accept_new_connection(server_socket);
// 		// handel_connection(client_socket, server_socket);
// 	}
// }