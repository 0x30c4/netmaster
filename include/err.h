#ifndef _ERR_H_
#define _ERR_H_

#include "common.h"
#include "stringlib.h"

#define SOCKETERROR (-1)

// // http error page 
// #define ERROR_PAGE "error/error.html"
// http status codes.

#define OK                    	0 // 200  every thing is ok
#define Bad_Request 			1 // 400  bad request/malformed request 
#define Not_Found 				2 // 404  not exist
#define Forbidden				3 // 403  file is not owned by server starting user.
#define Unauthorized   			4 // 401  not authorized to get the content.
#define Not_Acceptable			5 // 406  not send this to the client.
#define Request_URI_Too_long    6 // 414  Request-URI Too Long
#define Internal_Server_Error   7 // 500  server error

// status code len
#define OK_Len                    	6  // 200  every thing is ok
#define Bad_Request_Len 			15 // 400  bad request/malformed request 
#define Not_Found_Len 				13 // 404  not exist
#define Forbidden_Len				13 // 403  file is not owned by server starting user.
#define Unauthorized_Len   			15 // 401  not authorized to get the content.
#define Not_Acceptable_Len			18 // 406  not send this to the client.
#define Request_URI_Too_long_Len    16 // 414  Request-URI Too Long
#define Internal_Server_Error_Len   25 // 500  server error

#define CHECK(X) ({int __val = (X);(__val == -1 ? \
                    ({ fprintf(stderr, "ERROR (" __FILE__ ": %d ) -- %s \n", __LINE__, strerror(errno));\
                    exit(-1);-1;}):__val);})

#define FAIL_IF(EXP) ({if (EXP) {exit(EXIT_FAILURE);}})
#define FAIL_IF_MSG(EXP, MSG) ({if (EXP) {fprintf(stderr, MSG "\n");exit(EXIT_FAILURE);}})

// #define IF_FAIL_RET(EXP, REC) ({printf("REC line:%d | rec:%d | exp: %d\n", __LINE__, REC, EXP);if (EXP) {return REC;}})
#define IF_FAIL_RET(EXP, REC) ({if (EXP) {return REC;}})
// #define IF_FAIL_RET_NOT(EXP, REC) ({printf("!REC line:%d | rec:%d | exp: %d\n", __LINE__, REC, EXP);if (!EXP) {return REC;}})

int check(int, const char *);
void *errorPageSender(int client_socket, int error_code, char file_name[BUFSIZE]);
void print_usage(const char *PN);
int port_parser(char *p);
char *HeaderErrNo(int err);
char *HeaderErrNoStatusCode(int err);

extern int ERR_STATUS_CODE_LEN[9];
extern int ERR_STATUS_MSG_LEN[9];

#endif