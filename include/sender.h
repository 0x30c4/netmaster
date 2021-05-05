#ifndef _SENDER_H_
#define _SENDER_H_

#include "common.h"
#include "server.h"
#include "stringlib.h"
#include "err.h"

void * fileSender(int, const char *);
void headerSender(int , int, unsigned long long int, int);
#endif