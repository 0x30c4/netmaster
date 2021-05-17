#ifndef _STRINGLIB_H_
#define _STRINGLIB_H_

#include "common.h"
#include "err.h"

int endsWith(const char *, const char *);
int startsWith(const char *, const char *);
char **split(const char *, const char *, int *);
int strpcmp(const char *, const char *, size_t);
void slice_str(const char str[BUFSIZE], char *, size_t , size_t);
ssize_t readLine(int fd, void *buffer, size_t n);
ssize_t readCRLF(int fd, void *buffer);
// int readLine(int, char buf[BUFSIZE]);
int readLineFP(FILE *, char *);

#endif