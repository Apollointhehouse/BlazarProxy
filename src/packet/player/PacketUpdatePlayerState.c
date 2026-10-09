#include "packet/player/PacketUpdatePlayerState.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketUpdatePlayerState_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketUpdatePlayerState* data = packet->data;

    LOG_DEBUG("PacketUpdatePlayerState(state=%d)", data->state);

    return 1;
}

static void PacketUpdatePlayerState_destroy(const Packet* nonnull packet) {
    const PacketUpdatePlayerState* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketUpdatePlayerState_read(const Connection* nonnull connection) {
    PacketUpdatePlayerState* data = calloc(1, sizeof(PacketUpdatePlayerState));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketUpdatePlayerState_vtable();

    if (
        connection_read_i8(connection, &data->state) <= 0
    ) {
        PacketUpdatePlayerState_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketUpdatePlayerState_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketUpdatePlayerState* data = packet->data;

    connection_write_i8(connection, data->state);
}

const Packet_VTable* nonnull PacketUpdatePlayerState_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketUpdatePlayerState_read,
        .write = PacketUpdatePlayerState_write,
        .destroy = PacketUpdatePlayerState_destroy,
        .handle = PacketUpdatePlayerState_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketUpdatePlayerState_factory() {
    static const PacketFactory factory = {
        .read = PacketUpdatePlayerState_read
    };

    return &factory;
}
