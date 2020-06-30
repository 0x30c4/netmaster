#include "common.h"

int endsWith(const char *, const char *);
int startsWith(const char *, const char *);
char **split(const char *, const char *, int *);
int stringcmp(const char *, const char *);
void slice_str(const char str[BUFSIZE], char *, size_t , size_t);
ssize_t readLine(int fd, void *buffer, size_t n);
// int readLine(int, char buf[BUFSIZE]);
int readLineFP(FILE *, char *);
void stringcpy(char dest[BUFSIZE], char *str);
char *strlow(char str[BUFSIZE]);
bool isInArray(char **arry, char com[BUFSIZE], int array_size);
int contentLength(char buf[BUFSIZE / 2]);
