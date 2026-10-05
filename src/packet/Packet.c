#include "packet/Packet.h"
#include "packet/handshake/PacketDisconnect.h"
#include "packet/handshake/PacketPingHandshake.h"
#include "packet/handshake/PacketHandshake.h"

static const PacketFactory* packets[256];

void register_packets() {
    register_packet(002, PacketHandshake_factory());
    register_packet(254, PacketPingHandshake_factory());
    register_packet(255, PacketDisconnect_factory());
}

const PacketFactory* get_packet_factory(const uint8_t id) {
    const PacketFactory* factory = packets[id];
    if (!factory) return NULL;
    if (!factory->read) return NULL;

    return factory;
}

void register_packet(const uint8_t id, const PacketFactory* factory) {
    packets[id] = factory;
}