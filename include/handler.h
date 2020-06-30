#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "common.h"
#include "handler.h"
#include "parser.h"
#include "sender.h"
#include "server.h"
#include "err.h"

// parsing, what kind of request is made by the slave
void httpRequestHandler(int, int);
void *handle_connection(void*);
void *httpReqestParser(char *header, unsigned short *is_get_post_con_type_len, short *ret_code, short *,  bool *);
int fileChecker(char *, unsigned long long int *, short *);
int requestValidity(unsigned short *);
int postDataHandler(char buf[BUFSIZE]);


const char FILE_EXT[9][6];
char *PARSED_FROM_HEADERS[3]; //file, Cookie, POST data, content length.

#endif