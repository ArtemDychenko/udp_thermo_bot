#ifndef THERMO_PACKET_H
#define THERMO_PACKET_H

#include <stdint.h>
// timestamp dla 2024-01-01 zeby nie miec problem 2038 roku
#define EPOCH_2024_OFFSET 1704067200

typedef struct
{
    uint32_t timestamp;
    uint16_t temperature;
    uint8_t power_status;
    uint8_t measurement_id;
    uint8_t checksum;
} ThermoPacket;

ThermoPacket generate_packet(uint8_t measurement_id);
uint8_t calculate_checksum(ThermoPacket* pkt);

#endif
