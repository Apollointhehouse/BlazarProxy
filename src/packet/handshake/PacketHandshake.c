#include "packet/handshake/PacketHandshake.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"

static ssize_t PacketHandshake_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketHandshake* data = packet->data;

    STRING_UTF8_TO_C_STR(username, data->username);

    printf("PacketHandshake(username=%s)\n",username);

    return 1;
}

static void PacketHandshake_destroy(const Packet* nonnull packet) {
    const PacketHandshake* data = packet->data;

    data->username->destroy(data->username);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketHandshake_read(const Connection* nonnull connection) {
    PacketHandshake* data = calloc(1, sizeof(PacketHandshake));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketHandshake_vtable();

    if (!data) return NULL;

    if (
        connection_read_str_utf8(connection, &data->username) <= 0
    ) {
        PacketHandshake_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketHandshake_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketHandshake* data = packet->data;

    connection_write_str_utf8(connection, data->username);
}

const Packet_VTable* nonnull PacketHandshake_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketHandshake_read,
        .write = PacketHandshake_write,
        .destroy = PacketHandshake_destroy,
        .handle = PacketHandshake_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketHandshake_factory() {
    static const PacketFactory factory = {
        .read = PacketHandshake_read
    };

    return &factory;
}
