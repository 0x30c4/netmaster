#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "common.h"

#include "handler.h"
#include "parser.h"
#include "sender.h"
#include "server.h"
#include "err.h"

void postHandler(int, const char *, int);
void *handle_connection(void*);
void getHandler(int, const char *);
// parsing, what kind of request is made by the slave
void requestType(int client_socket, int server_socket);
char *headerParser(char *header, short *);
char *cookieHandler(char *cookie);
int fileChecker(char *fn, unsigned long long int *size);

#endif