#include "packet/world/PacketGameRule.h"

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketGameRule_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    LOG_DEBUG("PacketGameRule");

    return 1;
}

static void PacketGameRule_destroy(const Packet* nonnull packet) {
    const PacketGameRule* data = packet->data;

    if (data->tag) {
        NBT_destroy(data->tag);
    }

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketGameRule_read(const Connection* nonnull connection) {
    PacketGameRule* data = calloc(1, sizeof(PacketGameRule));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketGameRule_vtable();

    data->tag = malloc(sizeof(NBT));
    if (!data->tag) {
        PacketGameRule_destroy(packet);
        return NULL;
    }

    if (
        connection_read_nbt(connection, data->tag) <= 0
    ) {
        PacketGameRule_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketGameRule_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketGameRule* data = packet->data;

    connection_write_nbt(connection, data->tag);
}

const Packet_VTable* nonnull PacketGameRule_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketGameRule_read,
        .write = PacketGameRule_write,
        .destroy = PacketGameRule_destroy,
        .handle = PacketGameRule_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketGameRule_factory() {
    static const PacketFactory factory = {
        .read = PacketGameRule_read
    };

    return &factory;
}
