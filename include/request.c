#include "request.h"

// parsing, what kind of request is made by the slave
void requestType(int client_socket, int server_socket){
	char c[1];
	char buf[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
    bzero(&buf, sizeof(buf));
    bzero(&c, sizeof(c));

    /*
    	just reading the fast line of the request.
		for further checking. 
    */
    while((bytes_read = read(client_socket, c, 1))){
    	strncat(buf, c, 1);
    	msgsize += bytes_read;
    	if(msgsize > BUFSIZE-1 || c[0] == '\n') break;
    }

    // it's here for testing and debugging.    
    // printf("%s\n", buf);
    if (startsWith(buf, "exit")){
    	close(client_socket);
    	close(server_socket);
    	exit(0);
    }

    /*
    	checking the request type. 
    */
    getHandler(client_socket, buf);

    // if(startsWith(buf, GET)){
    // 	getHandler(client_socket, buf);
    // }else if (startsWith(buf, POST)){
    // 	postHandler(client_socket, buf, HTML);
    // }else{
    // 	write(client_socket, "ERROR", 5);
    // }

   	// write(client_socket, "\r\n\r\n", 4);
   
    fflush(stdout);
    // close(client_socket);
}



void postHandler(int client_socket, const char *buf, int flag){
	// parsing the first line of client request.
	// printf("%s\n", buf);
	headerParser(buf, _POST);

	//sending headers.
	headerSender(client_socket, 0);

	//sending file based on a request.
	fileSender(client_socket, INDEX_FILE);

 //    int split_len = 0;
 //    char **buf_split = split(buf, "\n", &split_len);

 //    for (int i = 0; i < split_len; ++i){
 //    	printf("%s ||", buf_split[i]);
 //    }

 //    free(buf_split);
	// fflush(stdout);
	// fileSender(client_socket, "slave.html");
	// headerSender(client_socket, flag);
	// Content-Length
}

void headerSender(int client_socket, int flag){
	write(client_socket, RESPONSE_GET_POST, sizeof(RESPONSE_GET_POST));
	write(client_socket, EOHL, sizeof(EOHL));
	
	write(client_socket, SERVER_DETAILS, sizeof(SERVER_DETAILS));
	write(client_socket, EOHL, sizeof(EOHL));
	
	if (flag == IMAGE_ICO){
		write(client_socket, FAVICON_HEADER, sizeof(FAVICON_HEADER));		
		write(client_socket, FAVICON_CONTLEN, sizeof(FAVICON_CONTLEN));		
	}else if (flag == HTML){
		write(client_socket, SERVER_CONTENT_TYPE, sizeof(SERVER_CONTENT_TYPE));
	}
	
	write(client_socket, EOHL, sizeof(EOHL));

   	write(client_socket, "Set-Cookie: _ga=asdayFTYFYJF>Mjasdj", 36);
	write(client_socket, EOHL, sizeof(EOHL));

	write(client_socket, CLOSE_GET_POST, sizeof(CLOSE_GET_POST));
	write(client_socket, EOH, sizeof(EOH));
	// printf("EOHL %d | EOH %d\n", sizeof(EOHL), sizeof(EOH));
	// write(client_socket, EOH, 4);
	// write(client_socket, "\n\n\n\n", 4);

}

/*
	this function is only for handling the connection 
	that are made by the USER from the browser.
*/
void getHandler(int client_socket, const char *buf){
	// parsing the first line of client request.
	// printf("%s\n", buf);
	headerParser(buf, _GET);
	

	char c[1];
	char buff[BUFSIZE];
	size_t bytes_read;
	int msgsize = 0;
    bzero(&buff, sizeof(buff));
    bzero(&c, sizeof(c));

    /*
    	just reading the fast line of the request.
		for further checking. 
    */
    while((bytes_read = read(client_socket, c, 1))){
    	strncat(buff, c, 1);
    	msgsize += bytes_read;
    	if(msgsize > BUFSIZE-1 || c[0] == 10) break;
    }

    printf("%s\n", buff);

	//sending headers.
	headerSender(client_socket, 0);
   	// write(1, "Set-Cookie: Basic=asdayFTYFYJF>Mjasdj", 37);

	//sending file based on a request.
	fileSender(client_socket, INDEX_FILE);
}

