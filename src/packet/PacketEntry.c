#include "packet/PacketEntry.h"

#include "packet/PacketPingHandshake.h"
#include <stdint.h>

static PacketEntry packets[256];

void register_packets() {
    register_packet(254, (PacketEntry) {
        .read = PacketPingHandshake_read,
        .write = PacketPingHandshake_write,
        .destroy = PacketPingHandshake_destroy,
        .handle = PacketPingHandshake_handle,
    });
}

PacketEntry* get_packet_entry(const uint8_t id) {
    return &packets[id];
}

void register_packet(const uint8_t id, const PacketEntry packet) {
    packets[id] = packet;
}