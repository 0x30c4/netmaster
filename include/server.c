#include "server.h"
#include "parser.h"

// Server respones headers.
const char RESPONSE_GET_POST[15] = "HTTP/1.1 200 OK";
const char SERVER_DETAILS[22] = "Server: NETMASTER/0.69";
const char SERVER_CONTENT_TYPE[38] = "Content-type: text/html; charset=UTF-8";
const char CLOSE_GET_POST[17] = "Connection: close";

const char FAVICON_HEADER[26] = "Content-Type: image/x-icon";
const char FAVICON_CONTLEN[20] = "Content-Length: 8894";


int setup_sever(short port, int backlog){
	// setting up the server.
	int server_socket;
	SA_IN server_addr;

	//creating a server socket.
	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket.");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	// binding a address.
	check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind Failed.");

	check(listen(server_socket, backlog), "Listen Failed.");

	// returning the server socket (file descriptor).
	return server_socket;
}


int accept_new_connection(int server_socket){
	int client_socket, addr_size = sizeof(SA_IN);
	SA_IN client_addr; 
	char client_addr_str[32];

	// need to change the output.
	printf("%s%sListenig on port %s%d%s\n", 
		BOLD, LIGHTGREEN, BLINK, SERVERPORT, RESETALL);

	// waiting for new slave.
	check((client_socket =
		accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)), 
		"Accept Failed.");

	//getting the slave address
	inet_ntop(AF_INET, &client_addr, client_addr_str, 100);

	// need to change this.
	printf("%s%s%s[+]%s %sA new slave is connected %s%s%s\n", 
		BOLD, BLINK, LIGHTGREEN, RESETALL, BOLD, LIGHTGREEN, client_addr_str, RESETALL);

	return client_socket;
}


void * handle_connection(void* args){
	// handling the slaves.
	// int client_socket = *((int*)args);
	// free(args);

	struct SERVER_CLIENT_FDS *server_client_fd = (struct SERVER_CLIENT_FDS*)args;


	// this function will parse the request from the slave.
	// requestType(client_socket);
	requestType(server_client_fd->client, server_client_fd->server);

    close(server_client_fd->client);
    return NULL;
}

// parsing, what kind of request is made by the slave
void requestType(int client_socket, int server_socket){
	char buf[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
    bzero(&buf, sizeof(buf));

    while((bytes_read = read(client_socket, buf+msgsize, sizeof(buf)-msgsize)))	{
    	msgsize += bytes_read;
    	if(msgsize > BUFSIZE-1 || buf[msgsize-1] == '\n') break; 
    	// if(msgsize > BUFSIZE-1 || endsWith(buf, EOH)) break;
    }

    if (startsWith(buf, "exit")){
    	close(server_socket);
    	exit(0);
    }

    if(startsWith(buf, GET)){
    	if (startsWith(buf, "GET /favicon.ico"))
    		getHandler(client_socket, buf, IMAGE_ICO);
    	else
    		getHandler(client_socket, buf, HTML);

    }else if (startsWith(buf, POST)){
    	postHandler(client_socket, buf, HTML);
    }else{
    	write(client_socket, "ERROR", 5);
    }

    fflush(stdout);
    // printf("%s", buf);
    close(client_socket);
    // printf("%s\n", buf);
}

/*
	this function is only for handling the connection 
	that are made by the USER from the browser.
*/
void getHandler(int client_socket, const char *buf, int flag){
	//sending headers.
	// sending the index.html file.
	headerSender(client_socket, flag);

	//index file sender.
	if (flag == HTML){
		fileSender(client_socket, INDEX_FILE);
	}else if (flag == IMAGE_ICO){
		fileSender(client_socket, FAVICON_ICO);		
	}
}

void postHandler(int client_socket, const char *buf, int flag){
    int buf_len = strlen(buf);
    printf("%d\n",buf_len);
    for (int i = 0; i < buf; ++i){
    	// if (buf[i] == '\n' && buf[i - 1] > 0 && buf[i + 1] < buf_len){
    	// 	printf("%s\n", buf[i + 1]);
    	// }
    	printf("%c", buf[i]);
    }

    printf("%s\n", buf);
	fflush(stdout);
	// headerSender(client_socket, flag);
	// Content-Length
}

void headerSender(int client_socket, int flag){
	write(client_socket, RESPONSE_GET_POST, sizeof(RESPONSE_GET_POST));
	write(client_socket, EOHL, 2);
	
	write(client_socket, SERVER_DETAILS, sizeof(SERVER_DETAILS));
	write(client_socket, EOHL, 2);
	
	if (flag == IMAGE_ICO){
		write(client_socket, FAVICON_HEADER, sizeof(FAVICON_HEADER));		
		write(client_socket, FAVICON_CONTLEN, sizeof(FAVICON_CONTLEN));		
	}else if (flag == HTML){
		write(client_socket, SERVER_CONTENT_TYPE, sizeof(SERVER_CONTENT_TYPE));
	}
	
	write(client_socket, EOHL, 2);

	write(client_socket, CLOSE_GET_POST, sizeof(CLOSE_GET_POST));
	write(client_socket, EOH, 4); 

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

    // printf("%s\n", actualpath);
    FILE *fp = fopen(actualpath, "r");
    if(fp == NULL){
    	printf("ERROR(open): %s\n", INDEX_FILE);
    	return NULL;
    }

    while((bytes_read = fread(buf, 1, BUFSIZE, fp)) > 0){
    	// printf("Sending %zu bytes\n", bytes_read);
    	write(client_socket, buf, bytes_read);
    }
    
    write(client_socket, EOH, 4);
    fclose(fp);
    // printf("Closing connection.\n");

    return NULL;
}


// void *fileReader(int client_socket){

// 	char buf[BUFSIZE];
// 	size_t bytes_read;
// 	int msgsize = 0;
// 	char actualpath[100];
//     bzero(&buf, sizeof(buf));
    
//     while((bytes_read = read(client_socket, buf+msgsize, sizeof(buf)-msgsize)))	{
//     	msgsize += bytes_read;
//     	if(msgsize > BUFSIZE-1 || buf[msgsize-1] == '\n') break;
//     }

//     check(bytes_read, "read error");
//     buf[msgsize-1] = 0;

//     printf("REQUEST: %s\n", buf);
//     fflush(stdout);
    
//     if(realpath(buf, actualpath) == NULL){
//     	printf("ERROR(bad path): %s\n", buf);
//     	return NULL;
//     }

//     FILE *fp = fopen(actualpath, "r");
//     if(fp == NULL){
//     	printf("ERROR(open): %s\n", buf);
//     	return NULL;
//     }

//     while((bytes_read = fread(buf, 1, BUFSIZE, fp)) > 0){
//     	printf("Sending %zu bytes\n", bytes_read);
//     	write(client_socket, buf, bytes_read);
//     }
    
//     write(client_socket, "\n", 1);

//     fclose(fp);
//     printf("Closing connection.\n");
 	
//  	return NULL;
// }



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