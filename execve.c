#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static struct __attribute__((packed))
{
    char prefix[32];
    char datefmt[32];
    char cmd[64];
} cmd = {"date: ", "\%Y-\%m-\%d \%H:\%M:\%S", "/root/libdft64/date.bin"};

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

int exec_cmd(char *buf)
{
    int pid;
    int p[2];
    size_t i;
    char *argv[3];

    for (i = 0; i < strlen(buf); i++)
    {
        if (buf[i] == '\n')
        {
            cmd.prefix[i] = '\0';
            break;
        }
        cmd.prefix[i] = buf[i];
    }

    argv[0] = cmd.cmd;
    argv[1] = cmd.datefmt;
    argv[2] = NULL;

    if (pipe(p) < 0)
    {
        perror("(execve-test) failed to open pipe");
        return -1;
    }

    switch (pid = fork())
    {
    case -1: /* Error */
        perror("(execve-test) fork failed");
        return -1;
    case 0: /* Child */
        printf("(execve-test/child) execv: %s %s\n", argv[0], argv[1]);
        fflush(stdout);

        close(1);
        dup(p[1]);
        close(p[0]);

        printf("%s", cmd.prefix);
        fflush(stdout);
        execv(argv[0], argv);
        perror("(execve-test/child) execve failed");
        kill(getppid(), SIGINT);
        exit(1);
    default: /* Parent */
        close(p[1]);
        return p[0];
    }

    return -1;
}

int main(int argc, char *argv[])
{
    printf("execve start.....\n");
    FILE *fp;
    int child_fd;
    char buf[4096];
    socklen_t addrlen;
    struct sockaddr_storage addr;

    int sockfd = open_socket("localhost", "9999");
    if (sockfd < 0)
    {
        fprintf(stderr, "(execve-test) failed to open socket\n");
        return 1;
    }

    addrlen = sizeof(addr);
    if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &addrlen) < 0)
    {
        fprintf(stderr, "(execve-test) recvfrom failed\n");
        return 1;
    }

    if ((child_fd = exec_cmd(buf)) < 0)
    {
        fprintf(stderr, "(execve-test) failed to exec command: %s\n", buf);
        return 1;
    }
    fp = fdopen(child_fd, "r");

    while (fgets(buf, sizeof(buf), fp))
    {
        sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&addr, addrlen);
    }

    fclose(fp);

    return 0;
}
