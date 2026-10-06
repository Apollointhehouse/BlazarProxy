#include "packet/player/PacketPlayerConfig.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"

static ssize_t PacketPlayerConfig_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketPlayerConfig* data = packet->data;

    printf("PacketPlayerConfig(entity_id=%d, config=%d)\n", data->entity_id, data->config);

    return 1;
}

static void PacketPlayerConfig_destroy(const Packet* nonnull packet) {
    const PacketPlayerConfig* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketPlayerConfig_read(const Connection* nonnull connection) {
    PacketPlayerConfig* data = calloc(1, sizeof(PacketPlayerConfig));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketPlayerConfig_vtable();

    if (
        connection_read_i32(connection, &data->entity_id) <= 0 ||
        connection_read_i16(connection, &data->config) < 0
    ) {
        PacketPlayerConfig_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketPlayerConfig_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketPlayerConfig* data = packet->data;

    connection_write_i32(connection, data->entity_id);
    connection_write_i16(connection, data->config);
}

const Packet_VTable* nonnull PacketPlayerConfig_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketPlayerConfig_read,
        .write = PacketPlayerConfig_write,
        .destroy = PacketPlayerConfig_destroy,
        .handle = PacketPlayerConfig_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketPlayerConfig_factory() {
    static const PacketFactory factory = {
        .read = PacketPlayerConfig_read
    };

    return &factory;
}
