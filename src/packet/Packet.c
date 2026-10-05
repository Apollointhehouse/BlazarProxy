#include "packet/Packet.h"
#include "packet/handshake/PacketDisconnect.h"
#include "packet/handshake/PacketPingHandshake.h"
#include "packet/handshake/PacketHandshake.h"
#include "packet/handshake/PacketLogin.h"

static const PacketFactory* nullable packets[256];

void register_packets() {
    register_packet(001, PacketLogin_factory());
    register_packet(002, PacketHandshake_factory());
    register_packet(254, PacketPingHandshake_factory());
    register_packet(255, PacketDisconnect_factory());
}

const PacketFactory* nullable get_packet_factory(const uint8_t id) {
    const PacketFactory* nullable factory = packets[id];
    if (!factory) return NULL;
    if (!factory->read) return NULL;

    return factory;
}

void register_packet(const uint8_t id, const PacketFactory* nonnull factory) {
    packets[id] = factory;
}