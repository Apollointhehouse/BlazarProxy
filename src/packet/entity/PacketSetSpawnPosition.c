#include "packet/entity/PacketSetSpawnPosition.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetSpawnPosition_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetSpawnPosition* data = packet->data;

    LOG_DEBUG("PacketSetSpawnPosition(x=%d, y=%d, z=%d)",data->x, data->y, data->z);

    return 1;
}

static void PacketSetSpawnPosition_destroy(const Packet* nonnull packet) {
    const PacketSetSpawnPosition* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetSpawnPosition_read(const Connection* nonnull connection) {
    PacketSetSpawnPosition* data = calloc(1, sizeof(PacketSetSpawnPosition));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketSetSpawnPosition_vtable();

    if (
        connection_read_i32(connection, &data->x) <= 0 ||
        connection_read_i32(connection, &data->y) <= 0 ||
        connection_read_i32(connection, &data->z) <= 0
    ) {
        PacketSetSpawnPosition_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketSetSpawnPosition_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetSpawnPosition* data = packet->data;

    connection_write_i32(connection, data->x);
    connection_write_i32(connection, data->y);
    connection_write_i32(connection, data->z);
}

const Packet_VTable* nonnull PacketSetSpawnPosition_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetSpawnPosition_read,
        .write = PacketSetSpawnPosition_write,
        .destroy = PacketSetSpawnPosition_destroy,
        .handle = PacketSetSpawnPosition_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetSpawnPosition_factory() {
    static const PacketFactory factory = {
        .read = PacketSetSpawnPosition_read
    };

    return &factory;
}
