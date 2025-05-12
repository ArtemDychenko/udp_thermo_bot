#include "ThermoPacket.h"
#include <stdlib.h>
#include <time.h>
#include <stddef.h>


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
