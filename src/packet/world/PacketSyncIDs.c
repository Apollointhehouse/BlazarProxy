#include "packet/world/PacketSyncIDs.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketSyncIDs_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketSyncIDs* data = packet->data;

    LOG_DEBUG("PacketSyncIDs(destination_id=%d, mapping_size=%d)",data->destination_id, data->mapping_size);

    return 1;
}

static void PacketSyncIDs_destroy(const Packet* nonnull packet) {
    const PacketSyncIDs* data = packet->data;

    if (!data->mapping) {
        free((void*)data);
        free((void*)packet);
        return;
    }

    for (int i = 0; i < data->mapping_size; i++) {
        StringUTF8* str = data->mapping[i];
        if (!str) continue;

        str->destroy(str);
    }

    free(data->mapping);

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketSyncIDs_read(const Connection* nonnull connection) {
    PacketSyncIDs* data = calloc(1, sizeof(PacketSyncIDs));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketSyncIDs_vtable();

    if (
        connection_read_i8(connection, &data->destination_id) <= 0 ||
        connection_read_i32(connection, &data->mapping_size) < 0
    ) goto fail;

    if (data->mapping_size <= 0) return packet;

    data->mapping = calloc(1, data->mapping_size * sizeof(StringUTF8*));

    if (!data->mapping) goto fail;

    for (int i = 0; i < data->mapping_size; i++) {
        int16_t id;

        if (connection_read_i16(connection, &id) <= 0) goto fail;
        if (connection_read_str_utf8(connection, &data->mapping[id]) <= 0) goto fail;
    }

    return packet;

    fail:
    PacketSyncIDs_destroy(packet);
    return NULL;
}

static void PacketSyncIDs_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketSyncIDs* data = packet->data;

    connection_write_i8(connection, data->destination_id);
    connection_write_i32(connection, data->mapping_size);

    for (int i = 0; i < data->mapping_size; i++) {
        const StringUTF8* nullable str = data->mapping[i];
        if (!str) continue;

        connection_write_i16(connection, (int16_t)i);
        connection_write_str_utf8(connection, str);
    }
}

const Packet_VTable* nonnull PacketSyncIDs_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketSyncIDs_read,
        .write = PacketSyncIDs_write,
        .destroy = PacketSyncIDs_destroy,
        .handle = PacketSyncIDs_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketSyncIDs_factory() {
    static const PacketFactory factory = {
        .read = PacketSyncIDs_read
    };

    return &factory;
}
