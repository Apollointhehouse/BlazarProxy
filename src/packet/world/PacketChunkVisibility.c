#include "packet/world/PacketChunkVisibility.h"

#include <stdio.h>
#include <stdlib.h>

#include "ConnectionContext.h"
#include "packet/Packet.h"
#include "packet/PacketFactory.h"
#include "packet/Packet_VTable.h"
#include "util/logging.h"

static ssize_t PacketChunkVisibility_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketChunkVisibility* data = packet->data;

    LOG_DEBUG(
        "PacketChunkVisibility("
        "chunk_x=%d, "
        "chunk_z=%d, "
        "player_added=%d)",
        data->chunk_x,
        data->chunk_z,
        data->player_added
    );

    return 1;
}

static void PacketChunkVisibility_destroy(const Packet* nonnull packet) {
    if (!packet) return;

    const PacketChunkVisibility* nonnull data = packet->data;
    if (!data) return;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketChunkVisibility_read(const Connection* nonnull connection) {
    PacketChunkVisibility* data = calloc(1, sizeof(PacketChunkVisibility));
    if (!data) {
        return NULL;
    }

    Packet* packet = malloc(sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketChunkVisibility_vtable();


    if (
        connection_read_i32(connection, &data->chunk_x) <= 0 ||
        connection_read_i32(connection, &data->chunk_z) <= 0 ||
        connection_read_i8(connection, &data->player_added) <= 0
    ) {
        perror("failed to read PacketChunkVisibility\n");
        PacketChunkVisibility_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketChunkVisibility_write(const Packet* nonnull packet, const Connection* connection) {
    const PacketChunkVisibility* data = packet->data;

    connection_write_i32(connection, data->chunk_x);
    connection_write_i32(connection, data->chunk_z);
    connection_write_i8(connection, data->player_added);
}

const Packet_VTable* nonnull PacketChunkVisibility_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketChunkVisibility_read,
        .write = PacketChunkVisibility_write,
        .destroy = PacketChunkVisibility_destroy,
        .handle = PacketChunkVisibility_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketChunkVisibility_factory() {
    static const PacketFactory factory = {
        .read = PacketChunkVisibility_read
    };

    return &factory;
}
