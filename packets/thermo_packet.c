#include "thermo_packet.h"
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


uint8_t calculate_checksum(ThermoPacket* pkt)
{
    uint8_t* ptr = (uint8_t*)pkt;
    size_t size = offsetof(ThermoPacket, checksum);
    uint8_t ckecksum = 0;
    for (size_t i = 0; i < size; i++)
    {
        ckecksum += ptr[i];
    }
    return ckecksum;
}

ThermoPacket generate_packet(uint8_t measurement_id)
{
    ThermoPacket pkt;
    uint32_t current_time = (uint32_t)time(NULL);
    pkt.timestamp = current_time - EPOCH_2024_OFFSET;
    pkt.temperature = (uint16_t)(200 + rand() % (1200 - 200 + 1));
    pkt.power_status = rand() % 2;
    pkt.measurement_id = measurement_id;
    pkt.checksum = calculate_checksum(&pkt);
    return pkt;
}

void print_packet(const ThermoPacket* pkt)
{
    char time_buffer[80];
    time_t actual_time = pkt->timestamp + EPOCH_2024_OFFSET;
    struct tm* timeinfo = localtime(&actual_time);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    printf("Thermometer: time: %s, temp: %.1f °C, power: %s, id: %u, checksum: %u\n",
           time_buffer,
           pkt->temperature / 10.0,
           pkt->power_status ? "Battery" : "Mains power supply",
           pkt->measurement_id,
           pkt->checksum);
}
