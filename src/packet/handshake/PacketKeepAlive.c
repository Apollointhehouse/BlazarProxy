#include "packet/handshake/PacketKeepAlive.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"

static ssize_t PacketKeepAlive_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    printf("PacketKeepAlive()");

    return 1;
}

static void PacketKeepAlive_destroy(const Packet* nonnull packet) {
    free((void*)packet);
}

static const Packet* PacketKeepAlive_read(const Connection* nonnull connection) {
    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        return NULL;
    }
    packet->data = NULL;
    packet->v = PacketKeepAlive_vtable();

    return packet;
}

static void PacketKeepAlive_write(const Packet* nonnull packet, const Connection* nonnull connection) {}

const Packet_VTable* nonnull PacketKeepAlive_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketKeepAlive_read,
        .write = PacketKeepAlive_write,
        .destroy = PacketKeepAlive_destroy,
        .handle = PacketKeepAlive_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketKeepAlive_factory() {
    static const PacketFactory factory = {
        .read = PacketKeepAlive_read
    };

    return &factory;
}
