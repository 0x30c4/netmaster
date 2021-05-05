#ifndef _HEADERPARSER_H_
#define _HEADERPARSER_H_

#include <common.h>
#include <stringlib.h>
#include <err.h>

int GetParser(const char* );
int PathChecker(const char * path, char * req_file);

extern char SERVER_ROOT[PATH_MAX];
extern int SERVER_ROOT_LEN = 0;

#endif