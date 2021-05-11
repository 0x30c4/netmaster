#include "../include/headerparser.h"


// check if SERVER_ROOT is valid dir in the context of this program

// int UID = 0;

int GetParser(const char * line, SERVER_ROOT * SD){
	// header type 
	// todo change str to var for opt.
	if (!strpcmp(line, "GET", 3)) return CAN_NOT_HANDEL_THIS_REQ;
	if (!endsWith(line, "HTTP/1.1\r\n") && !endsWith(line, "HTTP/2\r\n")) return MALFORMED_HADER;
    
	// Find out where everything is 

    const char *start_of_path = strchr(line, ' ') + 1;
    const char *start_of_query = strchr(start_of_path, '?');
    const char *end_of_query = strchr(start_of_query, ' ');

	int path_len, query_len;
	path_len = start_of_query - start_of_path; 
	query_len = end_of_query - start_of_query;
	
	if (path_len >= MAX_URL - 1)
		return URL_TOO_LONG;

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
	if (!startsWith(path, "/"))
		return MALFORMED_HADER;
	
	if (strstr(path, "/./") != NULL || strstr(path, "/../") != NULL)
		return PATH_ATTACK;

	if (path_len + SD->len >= PATH_MAX - 1)
		return URL_TOO_LONG;

	char *file = calloc(SD->len + path_len + 1, 1);

	
	int rec = PathChecker(path, file, SD);
	if (rec != 0) return rec;

    printf("Query -> %s %ld\n", query, sizeof(query));
    printf("Path -> %s %ld\n", path, sizeof(path));
    printf("File -> %s %ld\n", file, sizeof(file));

	free(file);

	return 0;
}


int PathChecker(const char * path, char * req_file, SERVER_ROOT * SD){

	strncat(req_file, SD->path, SD->len);
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