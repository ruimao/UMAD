#ifndef UMADCLIENT_H
#define UMADCLIENT_H

//TODO: independent on platform
#include "getopt.h"
#define ARG_NONE no_argument
#define ARG_NULL no_argument
#define ARG_REQ required_argument
#define ARG_OPT optional_argument
#define getopt_long_a getopt_long
#define optarg_a optarg
#define option_a option

//lib from cplusplus
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <iomanip>
using namespace std;

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/times.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define MYPORT 7777
#define LISTEN_QUEUE 10
#define MAXLINE 1024

#define SA struct sockaddr


extern void sendrawdatatoserver(int *serverlist, int servernum, char *filename, int size, char *datatype, int fragmentlength);
extern void sendquerydatatoserver(int *serverlist, int servernum, char *filename, int size, char *datatype, int fragmentLength);

#endif
