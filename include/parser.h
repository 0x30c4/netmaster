#include "common.h"

int endsWith(const char *, const char *);
int startsWith(const char *, const char *);
char **split(const char *, const char *, int *);
int stringcmp(const char *, const char *);
void slice_str(const char str[BUFSIZE], char *, size_t , size_t);
int readLine(int, char *);
int readLineFP(FILE *, char *);