#include "../include/common.h"
#include "../include/handler.h"
#include "../include/parser.h"

// 414 (Request-URI Too Long)

int PathChecker(const char * path, char * req_file);
int GetParser(const char* line);
int strpcmp(const char * s1, const char * s2, size_t cmpto);

// check if SERVER_ROOT is valid dir in the context of this program
char SERVER_ROOT[PATH_MAX];

int main(int argc, char const *argv[]){
	// parse("GET /path/script.cgi?field1=value1&field2=value2&fbclid=IwAR0gw6sBY47CIVxty4Qw8brrUnTbpRrh-uaKaP4nXsT9NckRGKSCHeTWReo HTTP/1.1");

	bzero(SERVER_ROOT, PATH_MAX);
	char *SR = "/root/CodeZ/C/netmaster/server_root\0";
	strncpy(SERVER_ROOT, SR, 35);
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

int GetParser(const char * line){
	/* header type */
	// todo change str to var for opt.
	if (!strpcmp(line, "GET", 3)) return CAN_NOT_HANDEL_THIS_REQ;
	if (!endsWith(line, "HTTP/1.1\r\n") && !endsWith(line, "HTTP/2\r\n")) return MALFORMED_HADER;
    
	/* Find out where everything is */

    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
    const char *end_of_query = strchr(start_of_query, ' ');

	int path_len, query_len;
	path_len = start_of_query - start_of_path; 
	query_len = end_of_query - start_of_query;
	
	if (path_len >= MAX_URL - 1)
		return URL_TOO_LONG;

    /* Get the right amount of memory */
    char path[path_len];
    char query[query_len];
 
    /* Copy the strings into our memory */
    strncpy(path, start_of_path,  path_len);
    strncpy(query, start_of_query, query_len);

    /* Null terminators (because strncpy does not provide them) */
    path[sizeof(path)] = 0;
    query[sizeof(query)] = 0;

	// error checking 
	if (!startsWith(path, "/"))
		return MALFORMED_HADER;
	
	if (strstr(path, "/./") != NULL || strstr(path, "/../") != NULL)
		return PATH_ATTACK;

	*start_of_path++;
	bzero(path, path_len);
    strncpy(path, start_of_path,  path_len - 1);

	if (path_len + strlen(SERVER_ROOT) >= PATH_MAX - 1)
		return URL_TOO_LONG;

	char *file = NULL;
	PathChecker(path, file);
    /*Print */
    printf("%s %ld\n", query, sizeof(query));
    printf("%s %ld\n", path, sizeof(path));
    printf("%s %ld\n", file, sizeof(file));

	free(file);

	return 0;
}

int PathChecker(const char * path, char * req_file){

	printf("11\n");
	req_file = calloc(strlen(SERVER_ROOT) + strlen(path), 1);
	
	sprintf(req_file, "%s/%s", SERVER_ROOT, path);

	if(access(req_file, F_OK) == -1) return Not_Found;

	struct stat sb;

	stat(req_file, &sb);

	// printf("stat: %d\n", stat(req_file, &sb));
	// printf("\t%d\n", sb.st_mode & S_IFMT == S_IFREG);
	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  return Forbidden;
		case S_IFCHR:  return Forbidden;
		case S_IFDIR:
			sprintf(req_file, "%s/%s/%s", SERVER_ROOT, path, INDEX_FILE);
			if(access(req_file, F_OK) == -1) {
				free(req_file);
				return Not_Found;
			}
			// printf("%s\n", req_file);
			// return Forbidden; break;
		case S_IFIFO:  return Forbidden;
		case S_IFLNK:  return Forbidden;
		case S_IFREG:  break;
		case S_IFSOCK: return Forbidden;
    	default:       return Forbidden;
    }

	// printf("%s \n", req_file);
	return 0;

}

int strpcmp(const char * s1, const char * s2, size_t cmpto){

	for (size_t i = 0; i < cmpto; i++){
		if ((char)(*s1++) != (char)(*s2++))
			return FALSE;
	}
	
	return TRUE;
}
// 413 - The request has exceeded the max length allowed