#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "common.h"
#include "stringlib.h"
#include "sender.h"
#include "server.h"
#include "err.h"

// parsing, what kind of request is made by the slave


const char FILE_EXT[9][6];
char *PARSED_FROM_HEADERS[3]; //file, Cookie, POST data, content length.

#endif