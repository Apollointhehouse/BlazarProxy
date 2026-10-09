#include "packet/entity/PacketSetEntityData.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetEntityData_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetEntityData* data = packet->data;

    const int32_t items_count = data->unpacked_data ? (int32_t)data->unpacked_data->size : 0;

    LOG_DEBUG("PacketSetEntityData(entity_id=%d, items_count=%d)", data->entity_id, items_count);

    return 1;
}


static void PacketSetEntityData_destroy(const Packet* nonnull packet) {
    const PacketSetEntityData* data = packet->data;

    if (data->unpacked_data) {
        for (int i = 0; i < data->unpacked_data->size; i++) {
            if (data->unpacked_data->data[i]) EntityDataItem_destroy(data->unpacked_data->data[i]);
        }
        DynamicArray_EntityDataItem_destroy(data->unpacked_data);
    }
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetEntityData_read(const Connection* nonnull connection) {
    PacketSetEntityData* data = calloc(1, sizeof(PacketSetEntityData));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketSetEntityData_vtable();

    if (
        connection_read_i32(connection, &data->entity_id) <= 0 ||
        EntityData_unpack(connection, &data->unpacked_data) <= 0
    ) {
        PacketSetEntityData_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketSetEntityData_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetEntityData* data = packet->data;

    connection_write_i32(connection, data->entity_id);
    EntityData_pack(connection, data->unpacked_data->size, data->unpacked_data->data);
}

const Packet_VTable* nonnull PacketSetEntityData_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetEntityData_read,
        .write = PacketSetEntityData_write,
        .destroy = PacketSetEntityData_destroy,
        .handle = PacketSetEntityData_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetEntityData_factory() {
    static const PacketFactory factory = {
        .read = PacketSetEntityData_read
    };

    return &factory;
}
