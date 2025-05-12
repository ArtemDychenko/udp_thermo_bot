#include "packets/thermo_packet.h"
#include "utils/utils.h"
#include "utime.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t running = 1;

void sig_handler(int sig)
{
    running = 0;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        printf("Use '%s <server_ip> <server_port> <interval_sec>' to run it right\n", argv[0]);
        return 1;
    }

    signal(SIGINT, sig_handler);

    int server_portno, interval;
    if (validate_args(argv[2], argv[3], &server_portno, &interval) != 0)
    {
        return 1;
    }

    struct sockaddr_in server_addr;
    int sockfd = initialize_socket(argv[1], server_portno, &server_addr);
    if (sockfd < 0)
    {
        return 1;
    }

    uint8_t measurement_id = 1;
    while (running)
    {
        ThermoPacket pkt = generate_packet(measurement_id++);
        sendto(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        print_packet(&pkt);
        sleep(interval);
    }

    close(sockfd);
    return 0;
}
