#include "packet/auth/PacketAESSendKey.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketAESSendKey_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketAESSendKey* data = packet->data;

    STRING_UTF8_TO_C_STR(key, data->key);

    LOG_DEBUG("PacketAESSendKey(key=\"%s\")",key);

    return 1;
}

static void PacketAESSendKey_destroy(const Packet* nonnull packet) {
    const PacketAESSendKey* data = packet->data;

    data->key->destroy(data->key);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketAESSendKey_read(const Connection* nonnull connection) {
    PacketAESSendKey* data = calloc(1, sizeof(PacketAESSendKey));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketAESSendKey_vtable();

    if (
        connection_read_str_utf8(connection, &data->key) <= 0
    ) {
        PacketAESSendKey_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketAESSendKey_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketAESSendKey* data = packet->data;

    connection_write_str_utf8(connection, data->key);
}

const Packet_VTable* nonnull PacketAESSendKey_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketAESSendKey_read,
        .write = PacketAESSendKey_write,
        .destroy = PacketAESSendKey_destroy,
        .handle = PacketAESSendKey_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketAESSendKey_factory() {
    static const PacketFactory factory = {
        .read = PacketAESSendKey_read
    };

    return &factory;
}
