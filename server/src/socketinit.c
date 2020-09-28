#include "socketinit.h"

extern int socket_Init(const char *serverip, const char *port)
{
    int fd = 0;
    fd =socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in sin;
    socklen_t len = sizeof(struct sockaddr_in);
    memset(&sin, '\0', sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(serverip);
    sin.sin_port = htons(atoi(port));

    if(bind(fd, (struct sockaddr*)&sin, len)<0)
    {
        perror("bind");
        return -1;
    }

    if(listen(fd, LISTENPOOL) < 0)
    {
        perror("listen");
        return -1;
    }

    return fd;
}