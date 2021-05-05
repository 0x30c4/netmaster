#include "handler.h"

const char VALID_HEADERS_FROM_SLAVE[5][15] = {"GET\0", "POST\0", "Cookie\0", "Content-Length\0", "Content-Type\0"};

const char FILE_EXT[9][6] = {"html\0", "htm\0", "js\0", "css\0", "txt\0",
                            "png\0", "jpg\0", "jpge\0", "ico\0"};

char *PARSED_FROM_HEADERS[3] = {"\0", "\0", "\0"}; //file, Cookie, GET/POST data, content length.
char SPECIAL_DIR[BUFSIZE / 4] = "\0";
char PASSWORD[BUFSIZE / 128] = "\0";

// for bit masking the values 

int CONTENT_LEN = -1;
unsigned short GET_R   = 0x01;
unsigned short POST_R  = 0x02;
unsigned short CON_LEN = 0x04;
unsigned short CON_TYP = 0x08;
