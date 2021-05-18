#include "../include/handler.h"

// const char VALID_HEADERS_FROM_SLAVE[5][15] = {"GET\0", "POST\0", "Cookie\0", "Content-Length\0", "Content-Type\0"};

// const char FILE_EXT[9][6] = {"html\0", "htm\0", "js\0", "css\0", "txt\0",
//                             "png\0", "jpg\0", "jpge\0", "ico\0"};

// char *PARSED_FROM_HEADERS[3] = {"\0", "\0", "\0"}; //file, Cookie, GET/POST data, content length.
// char SPECIAL_DIR[BUFSIZE / 4] = "\0";
// char PASSWORD[BUFSIZE / 128] = "\0";

// for bit masking the values 

// int CONTENT_LEN = -1;
// unsigned short GET_R   = 0x01;
// unsigned short POST_R  = 0x02;
// unsigned short CON_LEN = 0x04;
// unsigned short CON_TYP = 0x08;

int SlaveHandler(int fd, SERVER_ROOT * server_data){
    
    // printf("%s | %ld\n", lfh[0]->header, lfh[0]->headerLen);
	char *header = calloc(MAX_HEADER, 1);

	int headerLineLen = 0, retCode = 0;

	for (size_t header_no = 0; header_no <= MAX_HEADERS; header_no++){
		headerLineLen = readCRLF(0, header);
		if (headerLineLen == 0) break;
		if (headerLineLen < 0){
			printf("%d || %s --", headerLineLen, header);
			break;
		}else{
			if (header_no == 0)
				retCode = GetParser(header, server_data);
			else
				retCode = ParseHeader(header, headerLineLen);

			if (retCode != OK){
				fprintf(stderr, "HEADER ERROR!| No: %ld| Err: %d | %s | [ %s ] | len : %d\n", header_no, retCode, HeaderErrNoStatusCode(retCode), header, headerLineLen);
				break;
			}
            // printf("%s", header);
		}
		// bzero(header, MAX_HEADER);
		header_no++;
	}

    return OK;
}