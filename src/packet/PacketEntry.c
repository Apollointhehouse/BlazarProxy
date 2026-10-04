#include "packet/PacketEntry.h"

#include "packet/PacketPingHandshake.h"
#include <stdint.h>

#include "packet/PacketDisconnect.h"

static PacketEntry packets[256];

void register_packets() {
    register_packet(254, PacketPingHandshake_vtable());
    register_packet(255, PacketDisconnect_vtable());
}

PacketEntry* get_packet_entry(const uint8_t id) {
    PacketEntry* entry = &packets[id];
    if (!entry->read) return NULL;

    return entry;
}

void register_packet(const uint8_t id, const PacketEntry packet) {
    packets[id] = packet;
}