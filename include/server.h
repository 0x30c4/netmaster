#ifndef _SERVER_H_
#define _SERVER_H_

#include "common.h"
#include "handler.h"
#include "err.h"


int setup_sever(short , int);
int accept_new_connection(int);
// void identifyClient(int);
#endif