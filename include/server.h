#include "common.h"
#include "err.h"

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

/*
	Server header macros for header array.
*/
#define FILE 0
#define COOKIE 1
#define CONTENT_LENGTH 2

#define GET 0
#define POST 1

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

struct SERVER_CLIENT_FDS{
	int server;
	int client;
};

int setup_sever(short , int);
int accept_new_connection(int);
void * handle_connection(void*);
void requestType(int, int);
void getHandler(int, const char *);
void postHandler(int, const char *, int);
// void identifyClient(int);
void * fileSender(int, const char *);
void headerSender(int, int);
void headerParser(const char *, short);