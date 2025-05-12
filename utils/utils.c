#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/socket.h>

int initialize_socket(const char* server_ip, int server_portno, struct sockaddr_in* server_addr)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Error while creating socket");
        return -1;
    }

    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(server_portno);

    if (inet_pton(AF_INET, server_ip, &server_addr->sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid IP address\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int validate_args(const char* server_portno_str, const char* interval_str, int* server_portno, int* interval)
{
    char* endptr;
    errno = 0;

    *server_portno = (int)strtol(server_portno_str, &endptr, 10);
    *interval = (int)strtol(interval_str, &endptr, 10);

    if (errno == ERANGE || endptr == server_portno_str || *endptr != '\0' ||
        *server_portno <= 0 || *server_portno > 65535)
    {
        fprintf(
            stderr, "Invalid server port number, it should be in range 1 to 65535(preferably choose after 1024)\n");
        return -1;
    }

    if (errno == ERANGE || endptr == interval_str || *endptr != '\0' || *interval <= 0)
    {
        fprintf(stderr, "Invalid interval, it should be positive number\n");
        return -1;
    }

    return 0;
}
