#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <signal.h>

#include "ThermoPacket.h"

volatile sig_atomic_t running = 1;

void sig_handler(int sig)
{
    running = 0;
}

int main(int argc, char* argv[])
{
    signal(SIGINT, sig_handler);

    int sockfd, server_portno, interval;
    struct sockaddr_in server_addr;
    const char* server_ip;

    if (argc != 4)
    {
        printf("Use '%s <server_ip> <server_port> <interval_sec>' to run it right\n", argv[0]);
        return 1;
    }

    server_ip = argv[1];
    server_portno = atoi(argv[2]);
    interval = atoi(argv[3]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Error while creating socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_portno);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);


    uint8_t measurement_id = 1;
    while (running)
    {
        ThermoPacket pkt = generate_packet(measurement_id++);
        sendto(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        char time_buffer[80];
        time_t actual_time = pkt.timestamp + EPOCH_2024_OFFSET;
        struct tm* timeinfo = localtime(&actual_time);
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        printf("Thermometer: time: %s, temp: %.1f °C, power: %s, id: %u, checksum: %u\n",
               time_buffer,
               pkt.temperature / 10.0,
               pkt.power_status ? "Battery" : "Mains power supply",
               pkt.measurement_id,
               pkt.checksum);

        sleep(interval);
    }

    close(sockfd);
    return 0;
}
