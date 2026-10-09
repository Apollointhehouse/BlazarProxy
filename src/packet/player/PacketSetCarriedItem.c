#include "packet/player/PacketSetCarriedItem.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetCarriedItem_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetCarriedItem* data = packet->data;

    LOG_DEBUG("PacketSetCarriedItem(id=%d)", data->id);

    return 1;
}

static void PacketSetCarriedItem_destroy(const Packet* nonnull packet) {
    const PacketSetCarriedItem* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetCarriedItem_read(const Connection* nonnull connection) {
    PacketSetCarriedItem* data = calloc(1, sizeof(PacketSetCarriedItem));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketSetCarriedItem_vtable();

    if (
        connection_read_i16(connection, &data->id) <= 0
    ) {
        PacketSetCarriedItem_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketSetCarriedItem_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetCarriedItem* data = packet->data;

    connection_write_i16(connection, data->id);
}

const Packet_VTable* nonnull PacketSetCarriedItem_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetCarriedItem_read,
        .write = PacketSetCarriedItem_write,
        .destroy = PacketSetCarriedItem_destroy,
        .handle = PacketSetCarriedItem_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetCarriedItem_factory() {
    static const PacketFactory factory = {
        .read = PacketSetCarriedItem_read
    };

    return &factory;
}
