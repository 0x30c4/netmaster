#include "parser.h"

// checks if a string ends with a special pattern of characters or sub-string
int endsWith(const char *str, const char *subend){
	int _str_len = strlen(str);
	int _subend_len = strlen(subend);

    // printf("strlen: %d || substrlen: %d \n", _str_len, _subend_len);

    if (_str_len < _subend_len) return 0;

    int _start_from = _str_len - _subend_len;

    int j = 0;
	
    for (int i = _start_from; i != _str_len; i++){
        if (str[i] != subend[j]) return 0;
        j++;
    }
	return 1;
}

// checks if a string starts with a special pattern of characters or sub-string.
int startsWith(const char *str, const char *substart){
    int _str_len = strlen(str);
    int _substart_len = strlen(substart);

    if (_str_len < _substart_len) return 0;

    int j = 0;
    
    for (int i = 0; i != _substart_len; i++){
        if (str[i] != substart[j]) return 0;
        j++;
    }

    return 1;
}

// compares 2 string if they are the samess.
int stringcmp(const char *str1, const char *str2){
    if(strlen(str1) != strlen(str2)) return 0;
    for (int i = 0; i < strlen(str1); ++i){
        if (str1[i] != str2[i]) return 0;
    }
    return 1;
}

// splits a string every time it finds a specific character. and returns a string array.  
char **split(const char *str, const char *d, int *len){
    char _str[BUFSIZE];
    char** result = 0;
    char *rest = NULL;
    char *token;
    int count = 0;
    bzero(&_str, BUFSIZE);
    strncpy(_str, str, strlen(str));

    for (token = strtok_r(_str, d, &rest);
        token != NULL;
        token = strtok_r(NULL, d, &rest)){
        // printf("%s\n", token);
        count++;
    }   

    strncpy(_str, str, strlen(str));
    result = malloc(sizeof(char*) * count + 1);

    if (result){
        size_t idx  = 0;

        // *(result + idx++) = "hello";

        for (token = strtok_r(_str, d, &rest);
            token != NULL;
            token = strtok_r(NULL, d, &rest)){
            
            if (idx < count){
                *(result + idx++) = strdup(token);
            }
        }

    }
    *len = count;
    return result;
}

/* Read characters from 'fd' until a newline is encountered. If a newline
  character is not encountered in the first (n - 1) bytes, then the excess
  characters are discarded. The returned string placed in 'buf' is
  null-terminated and includes the newline character if it was read in the
  first (n - 1) bytes. The function return value is the number of bytes
  placed in buffer (which includes the newline character if encountered,
  but excludes the terminating null byte). */
ssize_t readLine(int fd, void *buffer, size_t n){
    /*************************************************************************\
    *                  Copyright (C) Michael Kerrisk, 2020.                   *
    *                                                                         *
    * This program is free software. You may use, modify, and redistribute it *
    * under the terms of the GNU Lesser General Public License as published   *
    * by the Free Software Foundation, either version 3 or (at your option)   *
    * any later version. This program is distributed without any warranty.    *
    * See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
    \*************************************************************************/

    /* read_line.c

       Implementation of readLine().
    */
    /*********
     * 
     *********/
    ssize_t numRead;                    /* # of bytes fetched by last read() */
    size_t totRead;                     /* Total bytes read so far */
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;                       /* No pointer arithmetic on "void *" */

    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)         /* Interrupted --> restart read() */
                continue;
            else
                return -1;              /* Some other error */

        } else if (numRead == 0) {      /* EOF */
            if (totRead == 0)           /* No bytes read; return 0 */
                return 0;
            else                        /* Some bytes read; add '\0' */
                break;

        } else {                        /* 'numRead' must be 1 if we get here */
            if (totRead < n - 1) {      /* Discard > (n - 1) bytes */
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return totRead;
}

/* 
    reads from a file descriptor until it gets a line feed.
    and puts the readied buffer in buf.
    returns the number of bytes it have read.  
*/
// int readLine(int fd, char buf[BUFSIZE]){
//     bzero(buf, sizeof(buf));
//     char c[1];
//     size_t bytes_read;
//     int msgsize = 0;
//     bzero(&c, sizeof(c));
//     while((bytes_read = read(fd, c, sizeof(c)))) {
//         msgsize += bytes_read;
//         strncat(buf, c, sizeof(c));
//         if(msgsize > (BUFSIZE / 2) - 1 || c[0] == '\n' || bytes_read == 0) break;
//     return msgsize;
// }

/* 
    reads from a file pointer until it gets a line feed.
    and puts the readied buffer in buf.
    returns the number of bytes it have read.  
*/
int readLineFP(FILE * fp, char *buf){
    int size = 0;
    char c;
    while((c = fgetc(fp)) != '\n' && size < BUFSIZE){
        buf[size++] = c;
    }
    return size;
}

void slice_str(const char str[BUFSIZE], char * buffer, size_t start, size_t end){
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

void stringcpy(char dest[BUFSIZE], char *str){
    bzero(dest, BUFSIZE);
    int str_len = strlen(str);
    // if (str_len > BUFSIZE) return NULL;
    for (int i = 0; i < str_len; ++i){
        dest[i] = str[i];
    }
}

char *strlow(char str[BUFSIZE]){
    int i;

    // Iterate loop till last character of string
    for(i=0; str[i]!='\0'; i++)
    {
        if(str[i]>='A' && str[i]<='Z')
        {
            str[i] = str[i] + 32;
        }
    }

    // printf("Lower case string: %s", str);

    return str;
}


bool isInArray(char **arry, char com[BUFSIZE], int array_size){
    for (int i = 0; i < array_size; ++i){
        if (stringcmp(arry[i], com))
            return TRUE;
    }
    return FALSE;
}