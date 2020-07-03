#ifndef _ARG_PARS_H_
#define _ARG_PARS_H_

#include "common.h"
#include "parser.h"
#include "err.h"

void arg_parser(int argc, char *argv[], char __special_dir[BUFSIZE / 4], char __pass[BUFSIZE / 128], int *port);

#endif