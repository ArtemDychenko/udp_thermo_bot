#ifndef UTILS_H
#define UTILS_H

#include <netinet/in.h>

int initialize_socket(const char* server_ip, int server_portno, struct sockaddr_in* server_addr);
int validate_args(const char* server_portno_str, const char* interval_str, int* server_portno, int* interval);

#endif
