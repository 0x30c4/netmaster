#ifndef _SENDER_H_
#define _SENDER_H_

#include "common.h"
#include "server.h"
#include "parser.h"
#include "err.h"

void * fileSender(int, const char *);
void headerSender(int, int);

#endif