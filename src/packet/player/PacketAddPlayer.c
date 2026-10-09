#include "packet/player/PacketAddPlayer.h"
#include "packet/Packet.h"
#include <stdio.h>
#include <stdlib.h>
#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t PacketAddPlayer_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketAddPlayer* data = packet->data;

    STRING_UTF8_TO_C_STR(name, data->name);
    UUID_TO_C_STR(uuid, data->uuid);
    STRING_UTF16_TO_C_STR(nickname, data->nickname);
    STRING_UTF16_TO_C_STR(gamemode, data->gamemode);

    LOG_DEBUG(
        "PacketAddPlayer("
        "entity_id=%d, "
        "name=%s, "
        "uuid=%s, "
        "x_position=%d, "
        "y_position=%d, "
        "z_position=%d, "
        "rotation=%d, "
        "pitch=%d, "
        "current_item=%d, "
        "nickname=%s, "
        "chat_color=%d, "
        "player_config=%d, "
        "gamemode=%s"
        ")\n",
        data->entity_id,
        name,
        uuid,
        data->x_position,
        data->y_position,
        data->z_position,
        data->rotation,
        data->pitch,
        data->current_item,
        nickname,
        data->chat_color,
        data->player_config,
        gamemode
    );

    return 1;
}

static void PacketAddPlayer_destroy(const Packet* nonnull packet) {
    const PacketAddPlayer* data = packet->data;

    if (data->name) data->name->destroy(data->name);
    if (data->nickname) data->nickname->destroy(data->nickname);
    if (data->gamemode) data->gamemode->destroy(data->gamemode);
    if (data->held_object_tag) NBT_destroy(data->held_object_tag);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketAddPlayer_read(const Connection* nonnull connection) {
    PacketAddPlayer* data = calloc(1, sizeof(PacketAddPlayer));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketAddPlayer_vtable();

    data->held_object_tag = malloc(sizeof(NBT));

    if (
        connection_read_i32(connection, &data->entity_id) <= 0 ||
        connection_read_str_utf8(connection, &data->name) <= 0 ||
        connection_read_uuid(connection, &data->uuid) <= 0 ||
        connection_read_i32(connection, &data->x_position) <= 0 ||
        connection_read_i32(connection, &data->y_position) <= 0 ||
        connection_read_i32(connection, &data->z_position) <= 0 ||
        connection_read_i8(connection, &data->rotation) <= 0 ||
        connection_read_i8(connection, &data->pitch) <= 0 ||
        connection_read_i16(connection, &data->current_item) <= 0 ||
        connection_read_str_utf16(connection, &data->nickname) <= 0 ||
        connection_read_i8(connection, &data->chat_color) <= 0 ||
        connection_read_i16(connection, &data->player_config) <= 0 ||
        connection_read_str_utf16(connection, &data->gamemode) <= 0
    ) {
        PacketAddPlayer_destroy(packet);
        return NULL;
    }

    int8_t has_nbt;

    if (connection_read_i8(connection, &has_nbt) <= 0) {
        PacketAddPlayer_destroy(packet);
        return NULL;
    }

    if (has_nbt) {
        connection_read_nbt(connection, data->held_object_tag);
    } else {
        data->held_object_tag = NULL;
    }

    return packet;
}

static void PacketAddPlayer_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketAddPlayer* data = packet->data;

    connection_write_i32(connection, data->entity_id);
    connection_write_str_utf8(connection, data->name);
    connection_write_uuid(connection, data->uuid);
    connection_write_i32(connection, data->x_position);
    connection_write_i32(connection, data->y_position);
    connection_write_i32(connection, data->z_position);
    connection_write_i8(connection, data->rotation);
    connection_write_i8(connection, data->pitch);
    connection_write_i16(connection, data->current_item);
    connection_write_str_utf16(connection, data->nickname);
    connection_write_i8(connection, data->chat_color);
    connection_write_i16(connection, data->player_config);
    connection_write_str_utf16(connection, data->gamemode);

    if (data->held_object_tag) {
        connection_write_i8(connection, 1);
        connection_write_nbt(connection, data->held_object_tag);
    } else {
        connection_write_i8(connection, 0);
    }
}

const Packet_VTable* nonnull PacketAddPlayer_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketAddPlayer_read,
        .write = PacketAddPlayer_write,
        .destroy = PacketAddPlayer_destroy,
        .handle = PacketAddPlayer_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketAddPlayer_factory() {
    static const PacketFactory factory = {
        .read = PacketAddPlayer_read
    };

    return &factory;
}
