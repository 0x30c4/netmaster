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

// char * findChar(const char *str, const char *delimiter){
//     // int _str_len = strlen(str);
//     // for (int i = 0; i < _str_len - 1; ++i){
//     //     if (str[i] == delimiter){

//     //     }
//     // }
// }