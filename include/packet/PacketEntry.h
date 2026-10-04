#pragma once
#include <stdint.h>

#include "Connection.h"

typedef struct PacketEntry {
    void* (*read)(const Connection* connection);
    void (*write)(const void* packet, const Connection* connection);
    void (*destroy)(const void* packet);
    void (*handle)(const void* packet);
} PacketEntry;

void register_packets();
PacketEntry* get_packet_entry(uint8_t id);
void register_packet(uint8_t id, PacketEntry packet);