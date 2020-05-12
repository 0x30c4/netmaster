#include "common.h"

#define SERVERPORT 6969
#define SERVER_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

// struct SERVER_CLIENT_FDS{
// 	int server;
// 	int client;
// };

int setup_sever(short , int);
int accept_new_connection(int);
void * handel_connection(void*);