#include "../include/server.h"

int setup_sever(short port, int backlog){
	// setting up the server.
	int server_socket;
	SA_IN server_addr;

	//creating a server socket.
	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket.");

    bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(port);
	// binding a address.
	check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind Failed.");

	check(listen(server_socket, backlog), "Listen Failed.");

	// need to change the output.
	printf("%s%sListenig on port %s%d%s\n", 
		BOLD, LIGHTGREEN, BLINK, SERVERPORT, RESETALL);

	// returning the server socket (file descriptor).
	return server_socket;
}


int accept_new_connection(int server_socket){
	int client_socket, addr_size = sizeof(SA_IN);
	SA_IN client_addr; 
	char client_addr_str[INET_ADDRSTRLEN];
	bzero(&client_addr_str, sizeof(client_addr_str));
	// waiting for new slave.
	check((client_socket =
		accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)), 
		"Accept Failed.");

	//getting the slave address
	struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
	struct in_addr ipAddr = pV4Addr->sin_addr;
	inet_ntop(AF_INET, &ipAddr, client_addr_str, INET_ADDRSTRLEN);

	// need to change this.
	printf("%s%s%s[+]%s %sA new slave is connected %s%s:%d%s\n", 
		BOLD, BLINK, LIGHTGREEN, RESETALL, BOLD, LIGHTGREEN, client_addr_str, ntohs(client_addr.sin_port), RESETALL);

	return client_socket;
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