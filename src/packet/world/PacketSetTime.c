#include "packet/world/PacketSetTime.h"

#include <inttypes.h>

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetTime_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetTime* data = packet->data;

    LOG_DEBUG("PacketSetTime(time=%"PRId64")", data->time);

    return 1;
}

static void PacketSetTime_destroy(const Packet* nonnull packet) {
    const PacketSetTime* data = packet->data;

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetTime_read(const Connection* nonnull connection) {
    PacketSetTime* data = calloc(1, sizeof(PacketSetTime));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketSetTime_vtable();

    if (
        connection_read_i64(connection, &data->time) <= 0
    ) {
        PacketSetTime_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketSetTime_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetTime* data = packet->data;

    connection_write_i64(connection, data->time);
}

const Packet_VTable* nonnull PacketSetTime_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetTime_read,
        .write = PacketSetTime_write,
        .destroy = PacketSetTime_destroy,
        .handle = PacketSetTime_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetTime_factory() {
    static const PacketFactory factory = {
        .read = PacketSetTime_read
    };

    return &factory;
}
