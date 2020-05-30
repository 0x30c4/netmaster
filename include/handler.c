#include "handler.h"

char *PARSED_FROM_HEADERS[4]; //file, Cookie, GET/POST data, content length.
char VALID_HEADERS_FROM_SLAVE[5][15] = {"GET", "POST", "Cookie", "Content-Length", "Content-Type"};


// parsing, what kind of request is made by the slave
void requestType(int client_socket, int server_socket){
	char buf[BUFSIZE];
    while(TRUE){
        readLine(client_socket, buf);
        printf("%s", headerParser(buf));
        // if(endsWith(buf, EOHL) && strlen(buf) > 2) printf("%s", buf);
        if(stringcmp(buf, EOHL)) break;
    }

	headerSender(client_socket, HTML);
	fileSender(client_socket, INDEX_FILE);

    // close(client_socket);
    // close(server_socket);
    // exit(0);

    // it's here for testing and debugging.

    if (startsWith(buf, "exit")){
    	close(client_socket);
    	close(server_socket);
    	exit(0);
    }
  
    fflush(stdout);
    close(client_socket);
}


char *headerParser(const char *header){
   	if(!endsWith(header, EOHL)) return 0; 

    int len_split = 0;

    char **splited_header = split(header, " ", &len_split);

    char file[BUFSIZE / 2];
    bzero(&file, sizeof(file));
    
    for (int i = 0; i < len_split; ++i){
        if (startsWith(splited_header[i], "/") && 
         endsWith(header, "HTTP/1.1\r\n") && 
         strlen(splited_header[i]) > 2) {
            // checking for path trivial attack.
            if(startsWith(splited_header[i], "/.") ||
                startsWith(splited_header[i], "//")){
                printf("attack!!\n");
                return FALSE;
            }

            size_t _len = strlen(splited_header[i]);
            int count = 0;
            for (; count < _len; ++count){
                if(splited_header[i][count] == '?'){ //|| splited_header[i][count] == '/'){
                    _len = count - 1;
                    // printf("%d\n", count);
                    break;
                }
            }
            printf("%s", splited_header[i]);
            slice_str(splited_header[i], file, 1, _len);

            return file;
        }else if (startsWith(header, "Cookie:")){
            // printf("a%s\n", splited_header[i]);
        }
    }
    // printf("%s\n", PARSED_FROM_HEADERS[FILE_NAME]);
    return "asdasd";
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


/*
	this function is only for handling the connection 
	that are made by the USER from the browser.
*/
void getHandler(int client_socket, const char *buf){
	char file[1024];
	bzero(&file, sizeof(file));
	// parsing the first line of client request.
	// headerParser(buf, GET, file);

    printf("%s\n", file);

	//sending headers.
	headerSender(client_socket, HTML);

	// //sending file based on a request.
	fileSender(client_socket, INDEX_FILE);
}

void postHandler(int client_socket, const char *buf, int flag){
	// parsing the first line of client request.
	// printf("%s\n", buf);
	// headerParser(buf, _POST);

	//sending headers.
	headerSender(client_socket, HTML);

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