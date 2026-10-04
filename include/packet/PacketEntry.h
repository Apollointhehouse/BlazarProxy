#pragma once
#include <stdint.h>

#include "Connection.h"
#include "ConnectionContext.h"

typedef struct PacketEntry {
    void* (*read)(const Connection* connection);
    void (*write)(const void* packet, const Connection* connection);
    void (*destroy)(const void* packet);
    ssize_t (*handle)(const void* packet, const ConnectionContext* ctx);
} PacketEntry;

void register_packets();
PacketEntry* get_packet_entry(uint8_t id);
void register_packet(uint8_t id, PacketEntry packet);