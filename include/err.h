#ifndef _ERR_H_
#define _ERR_H_

#include "common.h"

#define SOCKETERROR (-1)

// file permission related errors.
#define ERRFILENOTEXIST (-2) // server.conf file syntax error.
#define ERRNOTREGFILE (-3) // if the requested file is not regular file
#define ERRISDIR (-4) // if the requested file is a directory 
#define ERRFILENOT (-5) // if file dose not exist
#define ERRFILENOTREAD (-6) // if file is not readable
#define ERRFILENOTOWNED (-6) // if file is not owned by the user who started the server.

// http status codes.
#define OK                    	0 // every thing is ok
#define Bad_Request 			1 // bad request/malformed request 
#define Unauthorized   			2 // not authorized to get the content.
#define Forbidden				3 // file is not owned by server starting user.
#define Not_Found 				4	// not exist
#define Not_Acceptable			5 // not send this to the client.
#define Internal_Server_Error   6 // server error

// http error page 
#define ERROR_PAGE "error/error.html"
// #define OK                    	200 // every thing is ok
// #define Bad_Request 			400 // bad request/malformed request 
// #define Unauthorized   			401 // not authorized to get the content.
// #define Forbidden				403 // file is not owned by server starting user.
// #define Not_Found 				404	// not exist
// #define Not_Acceptable			406 // not send this to the client.
// #define Internal_Server_Error   500 // server error

extern const char STATUS_CODE[7][26];

int check(int, const char *);
void *errorPageSender(int client_socket, int error_code, char file_name[BUFSIZE]);
// int checkIfNull(void *, const char *)
#endif