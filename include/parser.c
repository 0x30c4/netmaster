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
    char _str[4096];
    char** result = 0;
    char *rest = NULL;
    char *token;
    int count = 0;
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

/* 
    reads from a file descriptor until it gets a line feed.
    and puts the readied buffer in buf.
    returns the number of bytes it have read.  
*/
int readLine(int fd, char *buf){
    bzero(buf, sizeof(buf));
    char c[1];
    size_t bytes_read;
    int msgsize = 0;
    bzero(&c, sizeof(c));
    while((bytes_read = read(fd, c, sizeof(c)))) {
        msgsize += bytes_read;
        strncat(buf, c, sizeof(c));
        if(msgsize > (BUFSIZE / 2) - 1 || c[0] == '\n' || bytes_read == 0) break;
    
    }
    return msgsize;
}

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
