#include "packet/player/PacketMovePlayerPosRot.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketMovePlayerPosRot_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketMovePlayerPosRot* data = packet->data;

    LOG_DEBUG("PacketMovePlayerPosRot(x=%.3f, y=%.3f, z=%.3f, yaw=%.3f, pitch=%.3f, on_ground=%d)", data->x, data->y, data->z, data->yaw, data->pitch, data->on_ground);

    return 1;
}

static void PacketMovePlayerPosRot_destroy(const Packet* nonnull packet) {
    const PacketMovePlayerPosRot* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketMovePlayerPosRot_read(const Connection* nonnull connection) {
    PacketMovePlayerPosRot* data = calloc(1, sizeof(PacketMovePlayerPosRot));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketMovePlayerPosRot_vtable();

    if (
        connection_read_double(connection, &data->x) <= 0 ||
        connection_read_double(connection, &data->y) <= 0 ||
        connection_read_double(connection, &data->z) <= 0 ||
        connection_read_float(connection, &data->yaw) <= 0 ||
        connection_read_float(connection, &data->pitch) <= 0 ||
        connection_read_i8(connection, &data->on_ground) <= 0
    ) {
        PacketMovePlayerPosRot_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketMovePlayerPosRot_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketMovePlayerPosRot* data = packet->data;

    connection_write_double(connection, data->x);
    connection_write_double(connection, data->y);
    connection_write_double(connection, data->z);
    connection_write_float(connection, data->yaw);
    connection_write_float(connection, data->pitch);
    connection_write_i8(connection, data->on_ground);
}

const Packet_VTable* nonnull PacketMovePlayerPosRot_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketMovePlayerPosRot_read,
        .write = PacketMovePlayerPosRot_write,
        .destroy = PacketMovePlayerPosRot_destroy,
        .handle = PacketMovePlayerPosRot_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketMovePlayerPosRot_factory() {
    static const PacketFactory factory = {
        .read = PacketMovePlayerPosRot_read
    };

    return &factory;
}
