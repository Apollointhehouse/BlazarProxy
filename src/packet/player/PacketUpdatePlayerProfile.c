#include "packet/player/PacketUpdatePlayerProfile.h"

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketUpdatePlayerProfile_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketUpdatePlayerProfile* data = packet->data;

    STRING_UTF8_TO_C_STR(username, data->username);
    STRING_UTF8_TO_C_STR(nickname, data->nickname);
    UUID_TO_C_STR(uuid, data->uuid);

    LOG_DEBUG(
        "PacketUpdatePlayerProfile("
        "username=\"%s\", "
        "nickname=\"%s\", "
        "uuid=\"%s\", "
        "score=%d, "
        "chat_color=%d, "
        "is_online=%d, "
        "is_operator=%d)",
        username,
        uuid,
        nickname,
        data->score,
        data->chat_color,
        data->is_online,
        data->is_operator
    );
    return 1;
}

static void PacketUpdatePlayerProfile_destroy(const Packet* nonnull packet) {
    const PacketUpdatePlayerProfile* data = packet->data;

    data->username->destroy(data->username);
    data->nickname->destroy(data->nickname);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketUpdatePlayerProfile_read(const Connection* nonnull connection) {
    PacketUpdatePlayerProfile* data = calloc(1, sizeof(PacketUpdatePlayerProfile));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketUpdatePlayerProfile_vtable();

    if (
        connection_read_str_utf8(connection, &data->username) <= 0 ||
        connection_read_str_utf16(connection, &data->nickname) <= 0 ||
        connection_read_uuid(connection, &data->uuid) <= 0 ||
        connection_read_i32(connection, &data->score) <= 0 ||
        connection_read_i8(connection, &data->chat_color) <= 0 ||
        connection_read_i8(connection, &data->is_online) <= 0 ||
        connection_read_i8(connection, &data->is_operator) <= 0
    ) {
        PacketUpdatePlayerProfile_destroy(packet);
        return NULL;
    }
    return packet;
}

static void PacketUpdatePlayerProfile_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketUpdatePlayerProfile* data = packet->data;

    connection_write_str_utf8(connection, data->username);
    connection_write_str_utf16(connection, data->nickname);
    connection_write_uuid(connection, data->uuid);
    connection_write_i32(connection, data->score);
    connection_write_i8(connection, data->chat_color);
    connection_write_i8(connection, data->is_online);
    connection_write_i8(connection, data->is_operator);
}

const Packet_VTable* nonnull PacketUpdatePlayerProfile_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketUpdatePlayerProfile_read,
        .write = PacketUpdatePlayerProfile_write,
        .destroy = PacketUpdatePlayerProfile_destroy,
        .handle = PacketUpdatePlayerProfile_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketUpdatePlayerProfile_factory() {
    static const PacketFactory factory = {
        .read = PacketUpdatePlayerProfile_read
    };

    return &factory;
}
