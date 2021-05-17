#include "../include/stringlib.h"

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
int strpcmp(const char *s1, const char *s2, size_t cmpto){
	
    for (size_t i = 0; i < cmpto; i++){
		if ((char)(*s1++) != (char)(*s2++))
			return FALSE;
	}
	return TRUE;
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


ssize_t readCRLF(int fd, void *buffer){

    ssize_t numRead;                    /* # of bytes fetched by last read() */
    size_t totRead;                     /* Total bytes read so far */
    char *buf;
    char ch;

    if (MAX_HEADER <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;                       /* No pointer arithmetic on "void *" */

    totRead = 0;

    bool prev_cr = FALSE;
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
            if (totRead < MAX_HEADER - 1) {      /* Discard > (n - 1) bytes */
                totRead++;
                *buf++ = ch;
            }else
                return Request_URI_Too_long;

            if (ch == '\n' && prev_cr)
                break;
            else if ((ch == '\n' && !prev_cr) || (ch != '\n' && prev_cr))
                return Bad_Request;

            if (ch == '\r')
                prev_cr = TRUE;
            else if (ch != '\r')
                prev_cr = FALSE;
        }
    }
    *buf = '\0';

    // printf("<< %d - <<< ", endsWith(buffer, "\r\n"));
    // if (((char *)buffer)[totRead] != '\n' || ((char *)buffer)[totRead-1] != '\r' )
    //     return MALFORMED_HADER;

    return totRead;
}

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