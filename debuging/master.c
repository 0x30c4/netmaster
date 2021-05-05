#include "../include/common.h"
#include "../include/handler.h"
#include "../include/stringlib.h"
#include "../include/headerparser.h"


// 414 (Request-URI Too Long)


char SERVER_ROOT[PATH_MAX];
int SERVER_ROOT_LEN = 0;

int PathChecker(const char * path, char * req_file);


int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	// UID = getuid();

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
int PathChecker(const char * path, char * req_file){

	strncat(req_file, SERVER_ROOT, SERVER_ROOT_LEN);
	strcat(req_file, path);

	// if(access(req_file, F_OK) == -1) return Not_Found;

	struct stat sb;

	// printf("1 - > %s \n", req_file);

	if (stat(req_file, &sb) == -1)
		return Not_Found;

	// printf("%u", sb.st_uid);
    // checking if the file is owned by the user who started the server.
    if (UID != sb.st_uid) return Forbidden;

	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  return Forbidden;
		case S_IFCHR:  return Forbidden;
		case S_IFDIR:
			strcat(req_file, "/");
			strcat(req_file, INDEX_FILE);
		
			if(stat(req_file, &sb) == -1) {
				free(req_file);
				return Not_Found;
			}else{
				if ((sb.st_mode & S_IFMT) != S_IFREG)
					return Forbidden;
			}
			break;
		case S_IFIFO:  return Forbidden;
		case S_IFLNK:  return Forbidden;
		case S_IFREG:  break;
		case S_IFSOCK: return Forbidden;
    	default:       return Forbidden;
    }
	return 0;

}
// 413 - The request has exceeded the max length allowed