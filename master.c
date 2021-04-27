#include "include/server.h"
#include "include/argumentparser.h"

int main(int argc, char const *argv[]){
	int client_socket, port;
	struct SERVER_CLIENT_DATA server_client_data;

	// parsing the arguments
	arg_parser(argc, argv, &server_client_data.special_dir, &server_client_data.password, &port);

	// assigning the server socket to the struct
	server_client_data.server = setup_sever(SERVERPORT, SERVER_BACKLOG);

	while(TRUE){
		// Waiting for new slave.
		client_socket = accept_new_connection(server_client_data.server);

		server_client_data.client = client_socket;

		pthread_t t;

		pthread_create(&t, NULL, handle_connection, &server_client_data);

		// pthread_join(t, NULL);
	}
	return 0;
}
//[H[J