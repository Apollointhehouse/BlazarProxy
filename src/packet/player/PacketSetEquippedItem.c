#include "packet/player/PacketSetEquippedItem.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSetEquippedItem_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSetEquippedItem* data = packet->data;

    LOG_DEBUG("PacketSetEquippedItem(entity_id=%d, slot=%d, item_id=%d, item_meta=%d)", data->entity_id, data->slot, data->item_id, data->item_meta);

    return 1;
}

static void PacketSetEquippedItem_destroy(const Packet* nonnull packet) {
    const PacketSetEquippedItem* data = packet->data;

    if (data->item_data) NBT_destroy(data->item_data);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSetEquippedItem_read(const Connection* nonnull connection) {
    PacketSetEquippedItem* data = calloc(1, sizeof(PacketSetEquippedItem));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketSetEquippedItem_vtable();

    data->item_data = malloc(sizeof(NBT));

    if (
        connection_read_i32(connection, &data->entity_id) <= 0 ||
        connection_read_i16(connection, &data->slot) <= 0 ||
        connection_read_i16(connection, &data->item_id) <= 0 ||
        connection_read_i16(connection, &data->item_meta) <= 0 ||
        connection_read_nbt(connection, data->item_data) <= 0
    ) {
        PacketSetEquippedItem_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketSetEquippedItem_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSetEquippedItem* data = packet->data;

    connection_write_i32(connection, data->entity_id);
    connection_write_i16(connection, data->slot);
    connection_write_i16(connection, data->item_id);
    connection_write_i16(connection, data->item_meta);
    connection_write_nbt(connection, data->item_data);
}

const Packet_VTable* nonnull PacketSetEquippedItem_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSetEquippedItem_read,
        .write = PacketSetEquippedItem_write,
        .destroy = PacketSetEquippedItem_destroy,
        .handle = PacketSetEquippedItem_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSetEquippedItem_factory() {
    static const PacketFactory factory = {
        .read = PacketSetEquippedItem_read
    };

    return &factory;
}
