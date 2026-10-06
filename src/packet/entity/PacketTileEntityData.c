#include "packet/entity//PacketTileEntityData.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"

static ssize_t PacketTileEntityData_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketTileEntityData* data = packet->data;

    BYTES_TO_HEX_STR(tag, data->tag.buffer, data->tag.size);

    printf("PacketTileEntityData(tag=\"%s\")\n", tag);

    return 1;
}

static void PacketTileEntityData_destroy(const Packet* nonnull packet) {
    const PacketTileEntityData* data = packet->data;

    NBT_destroy(&data->tag);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketTileEntityData_read(const Connection* nonnull connection) {
    PacketTileEntityData* data = calloc(1, sizeof(PacketTileEntityData));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketTileEntityData_vtable();

    if (
        connection_read_nbt(connection, &data->tag) <= 0
    ) {
        PacketTileEntityData_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketTileEntityData_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketTileEntityData* data = packet->data;

    connection_write_nbt(connection, data->tag);
}

const Packet_VTable* nonnull PacketTileEntityData_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketTileEntityData_read,
        .write = PacketTileEntityData_write,
        .destroy = PacketTileEntityData_destroy,
        .handle = PacketTileEntityData_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketTileEntityData_factory() {
    static const PacketFactory factory = {
        .read = PacketTileEntityData_read
    };

    return &factory;
}
