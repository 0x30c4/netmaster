#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>                                                                                                                         
#include <sys/stat.h>                                                                                                                          
#include <fcntl.h>
#include <sys/sysmacros.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

// defining the end of header terminators .
#define EOHL "\r\n" // End Of Header Line.
#define EOH  "\r\n\r\n" // End Of Header.

// some server macros.
#define BUFSIZE 4096 // Buffer size.
#define GET 0
#define POST 1
#define COOKIE 2

// Defining true and false. 
#define TRUE 1 
#define FALSE 0

//Bash colors and other formatting.

//Colors
#define RED "\e[31m"
#define GREEN "\e[32m"
#define LIGHTGREEN "\e[32m"

//Formats.
#define RESETALL "\e[0m"
#define BOLD "\e[1m"
#define UNDERLINE "\e[4m"
#define BLINK "\e[5m"

// extern char *a[3];

// Server constants.
#define SERVERPORT 6969 // Port to listen on.
#define SERVER_BACKLOG 100 // After this much connection the slave will be in waiting list
#define INDEX_FILE "index.html" // This file will be sent to every browser client
#define FAVICON_ICO "favicon.ico" // This file will be sent to every browser client

/* 
	Some macros for file handling. 
	When a file is send to the browser this macros will be in use.
*/
#define HTML 0
#define IMAGE_PNG 1 
#define IMAGE_ICO 2
#define IMAGE_JPG 3

/*
	Server header macros for header array.
*/
#define FILE_NAME 0
#define CONTENT_LENGTH 2

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

struct SERVER_CLIENT_FDS{
	int server;
	int client;
};

#endif