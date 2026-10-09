#include "packet/player/PacketSetHeldObject.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetHeldObject_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetHeldObject* data = packet->data;

    LOG_DEBUG("PacketSetHeldObject(entity_id=%d)", data->entity_id);

    return 1;
}

static void PacketSetHeldObject_destroy(const Packet* nonnull packet) {
    const PacketSetHeldObject* data = packet->data;

    if (data->object_tag) NBT_destroy(data->object_tag);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetHeldObject_read(const Connection* nonnull connection) {
    PacketSetHeldObject* data = calloc(1, sizeof(PacketSetHeldObject));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketSetHeldObject_vtable();

    data->object_tag = malloc(sizeof(NBT));

    int8_t has_nbt;

    if (
        connection_read_i32(connection, &data->entity_id) <= 0 ||
        connection_read_i8(connection, &has_nbt) <= 0
    ) {
        PacketSetHeldObject_destroy(packet);
        return NULL;
    }

    if (has_nbt) {
        if (connection_read_nbt(connection, data->object_tag) <= 0) {
            PacketSetHeldObject_destroy(packet);
            return NULL;
        }
    } else {
        data->object_tag = NULL;
    }

    return packet;
}

static void PacketSetHeldObject_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetHeldObject* data = packet->data;

    connection_write_i32(connection, data->entity_id);

    if (data->object_tag) {
        connection_write_i8(connection, 1);
        connection_write_nbt(connection, data->object_tag);
    } else {
        connection_write_i8(connection, 0);
    }
}

const Packet_VTable* nonnull PacketSetHeldObject_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetHeldObject_read,
        .write = PacketSetHeldObject_write,
        .destroy = PacketSetHeldObject_destroy,
        .handle = PacketSetHeldObject_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetHeldObject_factory() {
    static const PacketFactory factory = {
        .read = PacketSetHeldObject_read
    };

    return &factory;
}
