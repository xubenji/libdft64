#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int open_socket(const char *node, const char *service)
{
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, "9999", &hints, &res) != 0)
    {
        return -1;
    }

    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
    {
        return -1;
    }
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
    {
        return -1;
    }

    return sockfd;
}

int main(int argc, char *argv[])
{
    execv(argv[0]);
    open_socket("localhost", "9999");
    return 0;
}