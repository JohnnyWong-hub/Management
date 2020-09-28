#ifndef __SOCKETINIT_H__
#define __SOCKETINIT_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define LISTENPOOL 10

extern int socket_Init(const char *serverip, const char *port);

#endif