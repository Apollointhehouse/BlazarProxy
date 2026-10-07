#include "packet/world/PacketBlockRegionUpdate.h"
#include <stdio.h>
#include <stdlib.h>
#include "Connection.h"
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketBlockRegionUpdate_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketBlockRegionUpdate* data = packet->data;

    LOG_DEBUG(
        "PacketBlockRegionUpdate("
        "x_position=%d, "
        "y_position=%d, "
        "z_position=%d, "
        "x_size=%d, "
        "y_size=%d, "
        "z_size=%d, "
        "chunk_size=%d)",
        data->x_position,
        data->y_position,
        data->z_position,
        data->x_size,
        data->y_size,
        data->z_size,
        data->chunk_size
    );

    return 1;
}

static void PacketBlockRegionUpdate_destroy(const Packet* nonnull packet) {
    if (!packet) return;

    const PacketBlockRegionUpdate* nonnull data = packet->data;
    if (!data) return;

    if (data->chunk) free(data->chunk);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketBlockRegionUpdate_read(const Connection* nonnull connection) {
    PacketBlockRegionUpdate* data = calloc(1, sizeof(PacketBlockRegionUpdate));
    if (!data) {
        return NULL;
    }

    Packet* packet = malloc(sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketBlockRegionUpdate_vtable();


    if (
        connection_read_i32(connection, &data->x_position) <= 0 ||
        connection_read_i16(connection, &data->y_position) <= 0 ||
        connection_read_i32(connection, &data->z_position) <= 0 ||
        connection_read_i8(connection, &data->x_size) <= 0 ||
        connection_read_i8(connection, &data->y_size) <= 0 ||
        connection_read_i8(connection, &data->z_size) <= 0 ||
        connection_read_i32(connection, &data->chunk_size) <= 0
    ) {
        perror("failed to read PacketBlockRegionUpdate\n");
        PacketBlockRegionUpdate_destroy(packet);
        return NULL;
    }

    data->chunk = calloc(1, data->chunk_size);

    if (connection_read(connection, data->chunk_size, data->chunk) <= 0) {
        perror("failed to read chunk data\n");
        PacketBlockRegionUpdate_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketBlockRegionUpdate_write(const Packet* nonnull packet, const Connection* connection) {
    const PacketBlockRegionUpdate* data = packet->data;

    connection_write_i32(connection, data->x_position);
    connection_write_i16(connection, data->y_position);
    connection_write_i32(connection, data->z_position);
    connection_write_i8(connection, data->x_size);
    connection_write_i8(connection, data->y_size);
    connection_write_i8(connection, data->z_size);
    connection_write_i32(connection, data->chunk_size);
    connection_write(connection, data->chunk_size, data->chunk);
}

const Packet_VTable* nonnull PacketBlockRegionUpdate_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketBlockRegionUpdate_read,
        .write = PacketBlockRegionUpdate_write,
        .destroy = PacketBlockRegionUpdate_destroy,
        .handle = PacketBlockRegionUpdate_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketBlockRegionUpdate_factory() {
    static const PacketFactory factory = {
        .read = PacketBlockRegionUpdate_read
    };

    return &factory;
}
