#include "packet/player/PacketPhotoMode.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketPhotoMode_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketPhotoMode* data = packet->data;

    LOG_DEBUG("PacketPhotoMode(disabled=%d)", data->disabled);

    return 1;
}

static void PacketPhotoMode_destroy(const Packet* nonnull packet) {
    const PacketPhotoMode* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketPhotoMode_read(const Connection* nonnull connection) {
    PacketPhotoMode* data = calloc(1, sizeof(PacketPhotoMode));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketPhotoMode_vtable();

    if (
        connection_read_i8(connection, &data->disabled) <= 0
    ) {
        PacketPhotoMode_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketPhotoMode_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketPhotoMode* data = packet->data;

    connection_write_i8(connection, data->disabled);
}

const Packet_VTable* nonnull PacketPhotoMode_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketPhotoMode_read,
        .write = PacketPhotoMode_write,
        .destroy = PacketPhotoMode_destroy,
        .handle = PacketPhotoMode_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketPhotoMode_factory() {
    static const PacketFactory factory = {
        .read = PacketPhotoMode_read
    };

    return &factory;
}
