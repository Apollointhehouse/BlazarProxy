#pragma once
#include <stdint.h>

#include "connection.h"

typedef struct PacketEntry {
    void* (*create)(const Connection* connection);
    void (*destroy)(void* packet);
    void (*handle)(const void* packet);
} PacketEntry;

void register_packets();
PacketEntry* get_packet_entry(uint8_t id);
void register_packet(uint8_t id, PacketEntry packet);