#include<common.h>
#include<handler.h>
#include<stringlib.h>
#include<headerparser.h>
// #include "headerparser.h"


// 414 (Request-URI Too Long)


int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	SERVER_ROOT server_data;

	server_data.uid = getuid();
	// printf("uid -> %d\n", server_data.uid);
	// printf("gid -> %d\n", getgid());

	char *SR = "/root/CodeZ/C/netmaster/server_root\0";
	
	server_data.len = strlen(SR);

	strncpy(server_data.path, SR, server_data.len);	


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
				rec = GetParser(header, &server_data);
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