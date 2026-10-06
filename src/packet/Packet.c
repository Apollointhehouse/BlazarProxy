#include "packet/Packet.h"
#include "packet/handshake/PacketDisconnect.h"
#include "packet/handshake/PacketPingHandshake.h"
#include "packet/handshake/PacketHandshake.h"
#include "packet/auth/PacketLogin.h"
#include "packet/auth/PacketAESSendKey.h"
#include "packet/container/PacketRecipeSync.h"
#include "packet/entity/PacketTileEntityData.h"
#include "packet/handshake/PacketKeepAlive.h"
#include "packet/misc/PacketCustomPayload.h"
#include "packet/player/PacketPlayerConfig.h"
#include "packet/world/PacketBlockRegionUpdate.h"

static const PacketFactory* nullable packets[256];

void register_packets() {
    register_packet(0, PacketKeepAlive_factory());
    register_packet(1, PacketLogin_factory());
    register_packet(2, PacketHandshake_factory());
    register_packet(36, PacketPlayerConfig_factory());
    register_packet(51, PacketBlockRegionUpdate_factory());
    register_packet(75, PacketRecipeSync_factory());
    register_packet(136, PacketAESSendKey_factory());
    register_packet(140, PacketTileEntityData_factory());
    register_packet(250, PacketCustomPayload_factory());
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
    if (packets[id]) {
        printf("Packet factory already registered for id: %d\n", id);
        return;
    }

    packets[id] = factory;
}