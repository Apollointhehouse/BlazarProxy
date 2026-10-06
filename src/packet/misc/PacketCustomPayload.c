#include "packet/misc/PacketCustomPayload.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"

static ssize_t PacketCustomPayload_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketCustomPayload* data = packet->data;

    BYTES_TO_HEX_STR(payload_str, data->data, data->size);
    STRING_UTF8_TO_C_STR(net_channel, data->net_channel);

    printf("PacketCustomPayload(net_channel=\"%s\", size=%d, data=%s)\n",  net_channel, data->size, payload_str);

    return 1;
}

static void PacketCustomPayload_destroy(const Packet* nonnull packet) {
    const PacketCustomPayload* data = packet->data;

    free(data->data);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketCustomPayload_read(const Connection* nonnull connection) {
    PacketCustomPayload* data = calloc(1, sizeof(PacketCustomPayload));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketCustomPayload_vtable();


    if (
        connection_read_str_utf8(connection, &data->net_channel) <= 0 ||
        connection_read_i32(connection, &data->size) < 0
    ) {
        PacketCustomPayload_destroy(packet);
        return NULL;
    }

    data->data = calloc(1, data->size);

    if (data->size <= 0) {
        return packet;
    }

    if (
        connection_read(connection, data->size, data->data) < 0
    ) {
        PacketCustomPayload_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketCustomPayload_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketCustomPayload* data = packet->data;

    connection_write_str_utf8(connection, data->net_channel);
    if (data->size > 0) {
        connection_write_i32(connection, data->size);
        connection_write(connection, data->size, data->data);
    } else {
        connection_write_i32(connection, 0);
    }
}

const Packet_VTable* nonnull PacketCustomPayload_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketCustomPayload_read,
        .write = PacketCustomPayload_write,
        .destroy = PacketCustomPayload_destroy,
        .handle = PacketCustomPayload_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketCustomPayload_factory() {
    static const PacketFactory factory = {
        .read = PacketCustomPayload_read
    };

    return &factory;
}
