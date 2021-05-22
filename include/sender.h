#ifndef _SENDER_H_
#define _SENDER_H_

#include "common.h"
#include "server.h"
#include "stringlib.h"
#include "err.h"

#define CONT_TYP_LEN_HTML 24
#define CONT_TYP_LEN_JS 22
#define CONT_TYP_LEN_CSS 23
#define CONT_TYP_LEN_TXT 25
#define CONT_TYP_LEN_PNG 9
#define CONT_TYP_LEN_JPG 9
#define CONT_TYP_LEN_JPGE 10
#define CONT_TYP_LEN_ICON 12
#define CONT_TYP_LEN_APP_OCT_STR 24

void sendResponse(int, SERVER_ROOT *, char *);
void *fileSender(int, const char *);
void headerSender(int, char *, long int, int);
int getFileType(char * );
void buildContTypeHeader(int, int);
void sendErrPage(int, int);
#endif