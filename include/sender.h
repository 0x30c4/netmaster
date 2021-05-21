#ifndef _SENDER_H_
#define _SENDER_H_

#include "common.h"
#include "server.h"
#include "stringlib.h"
#include "err.h"

void sendResponse(int, SERVER_ROOT *, char *);
void *fileSender(int, const char *);
void headerSender(int, char *, long int, int);
int getFileType(char * );
void buildContTypeHeader(int, int);
#endif