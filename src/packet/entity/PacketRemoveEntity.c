#include "packet/entity/PacketRemoveEntity.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketRemoveEntity_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketRemoveEntity* data = packet->data;

    LOG_DEBUG("PacketRemoveEntity(entity_id=%d)",data->entity_id);

    return 1;
}

static void PacketRemoveEntity_destroy(const Packet* nonnull packet) {
    const PacketRemoveEntity* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketRemoveEntity_read(const Connection* nonnull connection) {
    PacketRemoveEntity* data = calloc(1, sizeof(PacketRemoveEntity));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketRemoveEntity_vtable();

    if (
        connection_read_i32(connection, &data->entity_id) <= 0
    ) {
        PacketRemoveEntity_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketRemoveEntity_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketRemoveEntity* data = packet->data;

    connection_write_i32(connection, data->entity_id);
}

const Packet_VTable* nonnull PacketRemoveEntity_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketRemoveEntity_read,
        .write = PacketRemoveEntity_write,
        .destroy = PacketRemoveEntity_destroy,
        .handle = PacketRemoveEntity_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketRemoveEntity_factory() {
    static const PacketFactory factory = {
        .read = PacketRemoveEntity_read
    };

    return &factory;
}
