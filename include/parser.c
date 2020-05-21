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


void slice_str(const char * str, char * buffer, size_t start, size_t end){
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}
