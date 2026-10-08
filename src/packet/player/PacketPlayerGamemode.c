#include "packet/player/PacketPlayerGamemode.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketPlayerGamemode_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketPlayerGamemode* data = packet->data;
    STRING_UTF16_TO_C_STR(gamemode_id, data->gamemode_id);

    LOG_DEBUG("PacketPlayerGamemode(entity_id=%d, gamemode_id=\"%s\")", data->entity_id, gamemode_id);

    return 1;
}

static void PacketPlayerGamemode_destroy(const Packet* nonnull packet) {
    const PacketPlayerGamemode* data = packet->data;

    if (data->gamemode_id) data->gamemode_id->destroy(data->gamemode_id);

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketPlayerGamemode_read(const Connection* nonnull connection) {
    PacketPlayerGamemode* data = calloc(1, sizeof(PacketPlayerGamemode));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketPlayerGamemode_vtable();

    if (
        connection_read_i32(connection, &data->entity_id) <= 0 ||
        connection_read_str_utf16(connection, &data->gamemode_id) <= 0
    ) {
        PacketPlayerGamemode_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketPlayerGamemode_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketPlayerGamemode* data = packet->data;

    connection_write_i32(connection, data->entity_id);
    connection_write_str_utf16(connection, data->gamemode_id);
}

const Packet_VTable* nonnull PacketPlayerGamemode_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketPlayerGamemode_read,
        .write = PacketPlayerGamemode_write,
        .destroy = PacketPlayerGamemode_destroy,
        .handle = PacketPlayerGamemode_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketPlayerGamemode_factory() {
    static const PacketFactory factory = {
        .read = PacketPlayerGamemode_read
    };

    return &factory;
}
