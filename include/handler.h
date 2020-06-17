#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "common.h"
#include "handler.h"
#include "parser.h"
#include "sender.h"
#include "server.h"
#include "err.h"

// for bit masking the values 
// #define GET_R   = 0x01
// #define POST_R  = 0x02
// #define CON_LEN = 0x04
// #define CON_TYP = 0x08

// parsing, what kind of request is made by the slave
void httpRequestHandler(int, int);
void *handle_connection(void*);
void *httpReqestParser(char *header, unsigned short *is_get_post_con_type_len, int *ret_code, short *,  bool *);
char *cookieHandler(char *);
int fileChecker(char *, unsigned long long int *, int *);
int requestValidity(unsigned short *);

const char FILE_EXT[9][6];
char *PARSED_FROM_HEADERS[3]; //file, Cookie, POST data, content length.

#endif