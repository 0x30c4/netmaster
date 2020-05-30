#include "common.h"

int endsWith(const char *, const char *);
int startsWith(const char *, const char *);
char **split(const char *, const char *, int *);
int stringcmp(const char *str1, const char *str2);
void slice_str(const char str[BUFSIZE], char *, size_t , size_t);
int readLine(int fd, char *buf);