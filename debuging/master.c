#include<common.h>
#include<handler.h>
#include<stringlib.h>
#include<headerparser.h>
// #include "headerparser.h"


// 414 (Request-URI Too Long)

// int main(int argc, char const *argv[]){

// 	printf("%d\n", GetParser("qqqe"));
// 	return 0;
// }





int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	// UID = getuid();
	// char SERVER_ROOT[PATH_MAX];
	// int SERVER_ROOT_LEN;

	bzero(SERVER_ROOT, PATH_MAX);

	char *SR = "/root/CodeZ/C/netmaster/server_root\0";
	// char *SR = "/root/netmaster/server_root\0";
	strncpy(SERVER_ROOT, SR, 35);
	SERVER_ROOT_LEN = strlen(SERVER_ROOT);
	
	char header[MAX_HEADER];

	bzero(header, MAX_HEADER);

	int rec = 0, header_no = 0;

	// printf("%s", strstr("/rnd/../ad../asd..asd/..../..././.../..", "/../") != NULL);

	// exit(-1);
	while (TRUE){
		rec = readCRLF(0, header);
		if (rec < 0){
			printf("%d || %s --", rec, header);
			break;
		}else{
			if (header_no == 0){
				rec = GetParser(header);
				if (rec != 0){
					printf("HEADER ERROR! %d\n", rec);
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