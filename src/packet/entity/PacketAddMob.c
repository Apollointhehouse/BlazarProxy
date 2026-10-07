#include "packet/entity/PacketAddMob.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketAddMob_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketAddMob* data = packet->data;

    STRING_UTF8_TO_C_STR(nickname, data->nickname);

    int items_count = data->unpacked_data ? data->unpacked_data->size : 0;

    LOG_DEBUG("PacketAddMob(id=%d, type=%d, x=%d, y=%d, z=%d, yaw=%d, pitch=%d, unpacked_items_count=%d, nickname=\"%s\", chat_color=%d)",
              data->id,
              data->type,
              data->x,
              data->y,
              data->z,
              (int)data->yaw,
              (int)data->pitch,
              items_count,
              nickname,
              (int)data->chat_color);

    return 1;
}


static void PacketAddMob_destroy(const Packet* nonnull packet) {
    const PacketAddMob* data = packet->data;

    if (data->unpacked_data) {
        for (int i = 0; i < data->unpacked_data->size; i++) {
            if (data->unpacked_data->data[i]) EntityDataItem_destroy(data->unpacked_data->data[i]);
        }
        DynamicArray_EntityDataItem_destroy(data->unpacked_data);
    }
    if (data->nickname) data->nickname->destroy(data->nickname);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketAddMob_read(const Connection* nonnull connection) {
    PacketAddMob* data = calloc(1, sizeof(PacketAddMob));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketAddMob_vtable();

    if (
        connection_read_i32(connection, &data->id) <= 0 ||
        connection_read_i16(connection, &data->type) <= 0 ||
        connection_read_i32(connection, &data->x) <= 0 ||
        connection_read_i32(connection, &data->y) <= 0 ||
        connection_read_i32(connection, &data->z) <= 0 ||
        connection_read_i8(connection, &data->yaw) <= 0 ||
        connection_read_i8(connection, &data->pitch) <= 0 ||
        EntityData_unpack(connection, &data->unpacked_data) <= 0 ||
        connection_read_str_utf8(connection, &data->nickname) <= 0 ||
        connection_read_i8(connection, &data->chat_color) <= 0
    ) {
        PacketAddMob_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketAddMob_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketAddMob* data = packet->data;

    connection_write_i32(connection, data->id);
    connection_write_i16(connection, data->type);
    connection_write_i32(connection, data->x);
    connection_write_i32(connection, data->y);
    connection_write_i32(connection, data->z);
    connection_write_i8(connection, data->yaw);
    connection_write_i8(connection, data->pitch);
    EntityData_pack(connection, data->unpacked_data->size, data->unpacked_data->data);
    connection_write_str_utf8(connection, data->nickname);
    connection_write_i8(connection, data->chat_color);
}

const Packet_VTable* nonnull PacketAddMob_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketAddMob_read,
        .write = PacketAddMob_write,
        .destroy = PacketAddMob_destroy,
        .handle = PacketAddMob_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketAddMob_factory() {
    static const PacketFactory factory = {
        .read = PacketAddMob_read
    };

    return &factory;
}
