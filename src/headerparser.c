#include "../include/headerparser.h"

int GetParser(const char * line, SERVER_ROOT * SD){
	// check if the header type is get and then check if the header is valide
	IF_FAIL_RET ((!strpcmp(line, "GET", 3)), CAN_NOT_HANDEL_THIS_REQ);
	IF_FAIL_RET ((!endsWith(line, "HTTP/1.1\r\n") && !endsWith(line, "HTTP/2\r\n")), MALFORMED_HADER);
    
	// Find out where everything is 
    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
    const char *end_of_query = strchr(start_of_query, ' ');

	int path_len, query_len;
	path_len = start_of_query - start_of_path; 
	query_len = end_of_query - start_of_query;
	
	IF_FAIL_RET (path_len >= MAX_URL - 1, URL_TOO_LONG);

    // Get the right amount of memory 
    char path[path_len];
    char query[query_len];
 
    // Copy the strings into our memory
    strncpy(path, start_of_path,  path_len);
    strncpy(query, start_of_query, query_len);

    // Null terminators (because strncpy does not provide them) 
    path[path_len] = 0;
    query[query_len] = 0;

	// error checking
	IF_FAIL_RET (!startsWith(path, "/"), MALFORMED_HADER);
	
	IF_FAIL_RET ((strstr(path, "/./") != NULL) || (strstr(path, "/../") != NULL), PATH_ATTACK);

	IF_FAIL_RET (path_len + SD->len >= PATH_MAX - 1, URL_TOO_LONG);

	char *file = calloc(SD->len + path_len + 1, 1);

	int rec = PathChecker(path, file, SD);
	// printf("REC -> %d\n", rec);
	if (rec != OK){
		free(file);
		return rec;	
	} 

    printf("Query -> %s %ld\n", query, sizeof(query));
    printf("Path -> %s %ld\n", path, sizeof(path));
    printf("File -> %s %ld\n", file, sizeof(file));

	free(file);

	return 0;
}

/*
	first check if a directory or file. if file then check permission
	if directory then check if permission and try to find index.html
*/

int PathChecker(const char * path, char * req_file, SERVER_ROOT * SD){

	strncat(req_file, SD->path, SD->len);
	strcat(req_file, path);

	struct stat fileStat;

	if (stat(req_file, &fileStat) == -1){
		fprintf(stderr, "%s\n", strerror(errno));
		return errno;
	}
	
	// check if the file or directory is readable 
	IF_FAIL_RET (!(fileStat.st_mode & S_IRUSR), Forbidden); 

	// check if the file is a link 
	IF_FAIL_RET (S_ISLNK(fileStat.st_mode), Forbidden); 

	if ((S_ISDIR(fileStat.st_mode))){
		
		strcat(req_file, "/");
		strcat(req_file, INDEX_FILE);
		
		if(stat(req_file, &fileStat) == -1) {
			free(req_file);
			return errno;
		}else{
			IF_FAIL_RET (((fileStat.st_mode & S_IFMT) != S_IFREG), Forbidden);
		}
	}
	return 0;
}
// 413 - The request has exceeded the max length allowed