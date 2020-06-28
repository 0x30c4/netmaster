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
#include <sys/random.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

/*
	Server header macros and flags.
*/
#define BUFSIZE 4096 // Buffer size.

// PARSED_FROM_HEADERS array index. 
#define FILE_NAME 0
#define COOKIE_D 1
#define POST_D 2

// Defining true and false. 
#define TRUE 1 
#define FALSE 0

//Bash colors and other formatting.

//Colors
#define RED            "\e[31m"
#define GREEN          "\e[32m"
#define LIGHTGREEN     "\e[32m"

//Formats.
#define RESETALL       "\e[0m"
#define BOLD           "\e[1m"
#define UNDERLINE      "\e[4m"
#define BLINK          "\e[5m"

// Server constants.
#define SERVERPORT     6969 // Port to listen on.
#define SERVER_BACKLOG 100 // After this much connection the slave will be in waiting list
#define INDEX_FILE     "index.html" // This file will be sent to every browser client
#define FAVICON_ICO    "favicon.ico" // This file will be sent to every browser client

/* 
	Some macros for file handling. 
	When a file is send to the browser this macros will be in use.
*/
#define HTML           0
#define HTM            1
#define JS             2
#define CSS            3
#define TEXT           4
#define IMAGE_PNG      5 
#define IMAGE_JPG      6
#define IMAGE_JPGE     7
#define IMAGE_ICO      8
#define OCTET_STREAM   9

// VALID_HEADERS_FROM_SLAVE array element index
#define GET            0
#define POST           1
#define COOKIE         2
#define CONTENTLENGTH  3
#define CONTENTTYPE    4

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

struct SERVER_CLIENT_FDS{
	int server;
	int client;
};

// defining the end of header terminators .
extern const char EOHL[3]; // End Of Header Line.
extern const char EOH[5]; // End Of Header.

#endif