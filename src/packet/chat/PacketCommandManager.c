#include "packet/chat/PacketCommandManager.h"
#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketCommandManager_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketCommandManager* data = packet->data;

    STRING_UTF8_TO_C_STR(suggestions, data->suggestions);

    LOG_DEBUG("PacketCommandManager(suggestions=\"%s\")", suggestions);

    return 1;
}

static void PacketCommandManager_destroy(const Packet* nonnull packet) {
    const PacketCommandManager* data = packet->data;

    data->suggestions->destroy(data->suggestions);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketCommandManager_read(const Connection* nonnull connection) {
    PacketCommandManager* data = calloc(1, sizeof(PacketCommandManager));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketCommandManager_vtable();

    if (
        connection_read_str_utf8(connection, &data->suggestions) <= 0
    ) {
        PacketCommandManager_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketCommandManager_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketCommandManager* data = packet->data;

    connection_write_str_utf8(connection, data->suggestions);
}

const Packet_VTable* nonnull PacketCommandManager_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketCommandManager_read,
        .write = PacketCommandManager_write,
        .destroy = PacketCommandManager_destroy,
        .handle = PacketCommandManager_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketCommandManager_factory() {
    static const PacketFactory factory = {
        .read = PacketCommandManager_read
    };

    return &factory;
}
