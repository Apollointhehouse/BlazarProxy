#include "packet/player/PacketSetHotbarOffset.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetHotbarOffset_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetHotbarOffset* data = packet->data;

    LOG_DEBUG("PacketSetHotbarOffset(hotbar_offset=%d)", data->hotbar_offset);

    return 1;
}

static void PacketSetHotbarOffset_destroy(const Packet* nonnull packet) {
    const PacketSetHotbarOffset* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetHotbarOffset_read(const Connection* nonnull connection) {
    PacketSetHotbarOffset* data = calloc(1, sizeof(PacketSetHotbarOffset));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketSetHotbarOffset_vtable();

    if (
        connection_read_i8(connection, &data->hotbar_offset) <= 0
    ) {
        PacketSetHotbarOffset_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketSetHotbarOffset_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetHotbarOffset* data = packet->data;

    connection_write_i8(connection, data->hotbar_offset);
}

const Packet_VTable* nonnull PacketSetHotbarOffset_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetHotbarOffset_read,
        .write = PacketSetHotbarOffset_write,
        .destroy = PacketSetHotbarOffset_destroy,
        .handle = PacketSetHotbarOffset_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetHotbarOffset_factory() {
    static const PacketFactory factory = {
        .read = PacketSetHotbarOffset_read
    };

    return &factory;
}
