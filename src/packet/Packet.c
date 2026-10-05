#include "packet/Packet.h"
#include "packet/PacketDisconnect.h"
#include "packet/PacketPingHandshake.h"

static const PacketFactory* packets[256];

void register_packets() {
    register_packet(254, PacketPingHandshake_factory());
    register_packet(255, PacketDisconnect_factory());
}

const PacketFactory* get_packet_factory(const uint8_t id) {
    const PacketFactory* factory = packets[id];
    if (!factory->read) return NULL;

    return factory;
}

void register_packet(const uint8_t id, const PacketFactory* factory) {
    packets[id] = factory;
}