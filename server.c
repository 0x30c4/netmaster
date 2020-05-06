#include "include/server.h"
// #include "include/common.h"

int main(int argc, char const *argv[]){
	int server_socket = setup_sever(SERVERPORT, SERVER_BACKLOG);

	fd_set current_sockets, ready_sockets;

	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	while(1){
		ready_sockets = current_sockets;

		check(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "select error.");

		for (int i = 0; i < FD_SETSIZE; ++i){
			if(FD_ISSET(i, &ready_sockets)){
				if(i == server_socket){
					int client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
				}else{
					handel_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
		// int client_socket = accept_new_connection(server_socket);
		// handel_connection(client_socket);
	}
	return 0;
}
