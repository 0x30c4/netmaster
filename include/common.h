#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>                                                                                                                         
#include <sys/stat.h>                                                                                                                          
#include <fcntl.h>
#include <sys/sysmacros.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

// defining the end of header terminators .
#define EOHL "\r\n" // End Of Header Line.
#define EOH  "\r\n\r\n" // End Of Header.

// defining the end of Request terminators .
#define EORL "\n" // End Of Request Line.
#define EOR  "\n\n" // End Of Request.

// defining the headers
#define GET "GET"
#define POST "POST"

#define BUFSIZE 4096 // Buffer size.

// Defining true and false. 
#define TRUE 1 
#define FALSE 0

//Bash colors and other formatting.

//Colors
#define RED "\e[31m"
#define GREEN "\e[32m"
#define LIGHTGREEN "\e[32m"

//Formats.
#define RESETALL "\e[0m"
#define BOLD "\e[1m"
#define UNDERLINE "\e[4m"
#define BLINK "\e[5m"

#endif