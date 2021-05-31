#include "server.c"
// #include "../include/server.h"
// #include "../include/handler.h"
// #include "../include/err.h"
// #include "../include/sender.h"
// #include "../include/headerparser.h"

int main(int argc, char const *argv[]){
	int client_socket, server_socket, port;
	// struct SERVER_CLIENT_DATA server_client_data;

	SERVER_ROOT server_data;

	char *SR = "/root/CodeZ/C/netmaster/server_root\0";
	// char *SR = "/home/sanaf/CodeZ/netmaster/server_root\0";
	
	server_data.len = strlen(SR);

	strncpy(server_data.path, SR, server_data.len);

	// SlaveHandler(0, &server_data);

	// assigning the server socket to the struct
	server_socket = setup_sever(SERVERPORT, SERVER_BACKLOG);

	while(TRUE){
		// Waiting for new slave.
		client_socket = accept_new_connection(server_socket);

		SlaveHandler(client_socket, &server_data);

		close(client_socket);

		// server_client_data.client = client_socket;

		// pthread_t t;

		// pthread_create(&t, NULL, handle_connection, &server_client_data);

		// pthread_join(t, NULL);
	}
	return 0;
}
//[H[J
