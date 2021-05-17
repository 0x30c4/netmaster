#include<common.h>
#include<handler.h>
#include<stringlib.h>
#include<headerparser.h>

int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	SERVER_ROOT server_data;

	server_data.uid = getuid();
	// printf("uid -> %d\n", server_data.uid);
	// printf("gid -> %d\n", getgid());

	char *SR = "/home/sanaf/CodeZ/netmaster/server_root\0";
	
	server_data.len = strlen(SR);

	strncpy(server_data.path, SR, server_data.len);	


	char header[MAX_HEADER];

	bzero(header, MAX_HEADER);

	int headerLineLen = 0, header_no = 0, retCode = 0;

	while (TRUE){
		headerLineLen = readCRLF(0, header);
		if (headerLineLen == 0) break;
		if (headerLineLen < 0){
			printf("%d || %s --", headerLineLen, header);
			break;
		}else{
			if (header_no == 0)
				retCode = GetParser(header, &server_data);
			else
				retCode = ParseHeader(header, headerLineLen);

			if (retCode != OK){
				fprintf(stderr, "HEADER ERROR!| No: %d| Err: %d | %s | [ %s ] | len : %d\n", header_no, retCode, HeaderErrNoStatusCode(retCode), header, headerLineLen);
				break;
			}
			if (header_no == 10) break;
		}
		bzero(header, MAX_HEADER);
		header_no++;
	}
	return 0;
}
