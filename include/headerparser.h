#ifndef _HEADERPARSER_H_
#define _HEADERPARSER_H_

#include "common.h"
#include "stringlib.h"
#include "err.h"

int GetParser(const char*, SERVER_ROOT *);
int PathChecker(const char *, char *, SERVER_ROOT *);
/* int ParseHeader(const char *, int); */
#endif
