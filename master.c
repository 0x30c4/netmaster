// #include "include/server.h"
#include <server.h>

int main(int argc, char const *argv[]){
	int server_socket, client_socket;

	// creating server socket.
	server_socket = setup_sever(SERVERPORT, SERVER_BACKLOG);

	// test
	struct SERVER_CLIENT_FDS *server_client_fd;
	(*server_client_fd).server = server_socket;
	

	while(TRUE){
		// Waiting for new slave.
		client_socket = accept_new_connection(server_socket);
		
		//
		(*server_client_fd).client = client_socket;

		pthread_t t;

		// int *pclient = malloc(sizeof(int));

		// *pclient = client_socket;

		// pthread_create(&t, NULL, handle_connection, pclient);
		pthread_create(&t, NULL, handle_connection, server_client_fd);

		// pthread_join(t, NULL);
	    // break;
	}
	return 0;
}
//[H[J