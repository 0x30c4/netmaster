#include "../include/handler.h"

int SlaveHandler(int fd, SERVER_ROOT * server_data){
	char *header = calloc(MAX_HEADER, 1);
	char *path = calloc(MAX_URL - MAX_FILE_NAME, 1);
	int headerLineLen = 0, retCode = 0;

	for (size_t header_no = 0; header_no <= MAX_HEADERS; header_no++){
		headerLineLen = readCRLF(0, header);
		if (headerLineLen == 0) break;
		if (headerLineLen < 0){
			printf("%d || %s --", headerLineLen, header);
			break;
		}else{
			if (header_no == 0){
				retCode = GetParser(header, server_data, path);
				sendResponse(1, server_data, path);
            }
			if (retCode != OK){
				fprintf(stderr, "HEADER ERROR!| No: %ld| Err: %d | %s | [ %s ] | len : %d\n", header_no, retCode, HeaderErrNoStatusCode(retCode), header, headerLineLen);
				break;
			}
		}
		header_no++;
	}
	free(header);
	free(path);
    return OK;
}
