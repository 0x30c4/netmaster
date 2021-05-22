#include "../include/headerparser.h"

int GetParser(const char * line, SERVER_ROOT * SD, char * _path){
	// check if the header type is get and then check if the header is valide
	IF_FAIL_RET ((!strpcmp(line, "GET", 3)), Not_Acceptable);
	IF_FAIL_RET ((!endsWith(line, "HTTP/1.1\r\n") && !endsWith(line, "HTTP/2\r\n")), Bad_Request);
    
	// Find out where everything is 
    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
	const char *end_of_path = strchr(start_of_path, ' ');
    // const char *end_of_query = strchr(start_of_query, ' ');

	int path_len = 0;

	if (start_of_query == NULL)
		path_len = end_of_path - start_of_path;
	else
		path_len = start_of_query - start_of_path; 
	// int query_len = end_of_query - start_of_query;
	
	IF_FAIL_RET (path_len >= MAX_URL - 1, Request_URI_Too_long);

    // Get the right amount of memory 
    char path[path_len];
    // char query[query_len];
 
    // Copy the strings into our memory
    strncpy(path, start_of_path,  path_len);
    // strncpy(query, start_of_query, query_len);

    // Null terminators (because strncpy does not provide them) 
    path[path_len] = 0;
    // query[query_len] = 0;

	// error checking
	IF_FAIL_RET (!startsWith(path, "/"), Bad_Request);
	
	IF_FAIL_RET ((strstr(path, "/./") != NULL) || (strstr(path, "/../") != NULL), Not_Acceptable);

	IF_FAIL_RET (path_len + SD->len >= MAX_URL - MAX_FILE_NAME - 1, Request_URI_Too_long);

	// char *file = calloc(SD->len + path_len + 1, 1);	


	// int rec = PathChecker(path, file, SD);
	// printf("path ---> %s\n", path);
	// if (rec != OK){
	// 	free(file);
	// 	return rec;	
	// }

	memcpy(_path, path, path_len);
    // printf("Query -> %s %ld\n", query, sizeof(query));
    // printf("Path -> %s %ld\n", path, sizeof(path));
    // printf("File -> %s %ld %ld \n", file, strlen(file), SD->len + path_len + 1);

	// free(file);

	return OK;
}

/*
	first check if a directory or file. if file then check permission
	if directory then check if permission and try to find index.html
*/

int PathChecker(const char * path, char * req_file, long int *fsize, SERVER_ROOT * SD){

	struct stat fileStat;
	strncat(req_file, SD->path, SD->len);
	strcat(req_file, path);


	if (stat(req_file, &fileStat) == -1){
		fprintf(stderr, "%s | %d\n", strerror(errno), errno);
		if (errno == 2) return Not_Found;
	}
	
	// check if the file or directory is readable 
	IF_FAIL_RET (!(fileStat.st_mode & S_IRUSR), Forbidden); 

	// check if the file is a link 
	IF_FAIL_RET (S_ISLNK(fileStat.st_mode), Forbidden); 

	if ((S_ISDIR(fileStat.st_mode))){
		
		// printf("new len -> %ld\n", strlen(INDEX_FILE) + strlen(req_file) + 1);
		
		req_file = realloc(req_file, strlen(INDEX_FILE) + strlen(req_file) + 1);

		strcat(req_file, "/");
		strcat(req_file, INDEX_FILE);
		
		if(stat(req_file, &fileStat) == -1) {
			free(req_file);
			return errno;
		}else{
			IF_FAIL_RET (((fileStat.st_mode & S_IFMT) != S_IFREG), Forbidden);
		}
	}
	*fsize = fileStat.st_size;
	return OK;
}

// 413 - The request has exceeded the max length allowed
/* int ParseHeader(const char * header, int headerLen){ */

/* 	IF_FAIL_RET ((endsWith(header, EOHL) && headerLen == 2), OK); */

/* 	IF_FAIL_RET (!endsWith(header, EOHL), Bad_Request); */

/* 	return OK; */
/* } */
