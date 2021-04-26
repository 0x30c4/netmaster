#include "../include/common.h"
#include "../include/handler.h"
#include "../include/parser.h"

// 414 (Request-URI Too Long)

int parse(const char* line);
int strpcmp(const char * s1, const char * s2, size_t cmpto);

int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	char header[MAX_HEADER];

	bzero(header, MAX_HEADER);

	int rec = 0, header_no = 0;

	while (TRUE){
		rec = readCRLF(0, header);
		if (rec < 0){
			printf("%d || %s --", rec, header);
			break;
		}else{
			if (header_no == 0){
				rec = parse(header);
				if (rec != 0){
					printf("HEADER ERROR!\n");
					break;
				}
				break;
			}
		}
		bzero(header, MAX_HEADER);
		header_no++;
	}
	return 0;
}

int parse(const char * line){
    /* Find out where everything is */
	if (!strpcmp(line, "GET", 3)) return CAN_NOT_HANDEL_THIS_REQ;

    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
    const char *end_of_query = strchr(start_of_query, ' ');

    /* Get the right amount of memory */
    char path[start_of_query - start_of_path];
    char query[end_of_query - start_of_query];
 
    /* Copy the strings into our memory */
    strncpy(path, start_of_path,  start_of_query - start_of_path);
    strncpy(query, start_of_query, end_of_query - start_of_query);

    /* Null terminators (because strncpy does not provide them) */
    path[sizeof(path)] = 0;
    query[sizeof(query)] = 0;

	/* header type */

	

    /*Print */
    printf("%s %ld\n", query, sizeof(query));
    printf("%s %ld\n", path, sizeof(path));

	return 0;
}

int strpcmp(const char * s1, const char * s2, size_t cmpto){

	for (size_t i = 0; i < cmpto; i++){
		if ((char)(*s1++) != (char)(*s2++))
			return FALSE;
	}
	
	return TRUE;
}