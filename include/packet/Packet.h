#pragma once
#include <stdint.h>
#include <nullability.h>

typedef struct Packet_VTable Packet_VTable;
typedef struct PacketFactory PacketFactory;

typedef struct Packet {
    const Packet_VTable* nonnull v;
    const void* nonnull data;
} Packet;

void register_packets();
const PacketFactory* nullable get_packet_factory(uint8_t id);
void register_packet(uint8_t id, const PacketFactory* nonnull factory);