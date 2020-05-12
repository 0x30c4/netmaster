// #include "include/server.h"
#include <server.h>

int main(int argc, char const *argv[]){
	int server_socket, client_socket;

	server_socket = setup_sever(SERVERPORT, SERVER_BACKLOG);

	// test
	// struct SERVER_CLIENT_FDS *server_client_fd;
	
	// (*server_client_fd).server = server_socket;

while(1){	
	client_socket = accept_new_connection(server_socket);

	printf("%d\n", client_socket);
	pthread_t t;

	int *pclient = malloc(sizeof(int));

	*pclient = client_socket;

	pthread_create(&t, NULL, handel_connection, pclient);

	// pthread_join(t, NULL);
}
	return 0;
}
//[H[J