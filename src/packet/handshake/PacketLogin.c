#include "packet/handshake/PacketLogin.h"

#include <inttypes.h>

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"

static ssize_t PacketLogin_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketLogin* data = packet->data;

    STRING_UTF8_TO_C_STR(username, data->username);
    STRING_UTF8_TO_C_STR(public_key, data->public_key);

    UUID_TO_C_STR(uuid, data->uuid);

    printf(
        "PacketLogin("
        "proto_or_player_id=%d, "
        "username=\"%s\", "
        "uuid=\"%s\", "
        "public_key=\"%s\", "
        "world_seed=%"PRId64", "
        "dimension_id=%d, "
        "world_type_id=%d, "
        "packet_delay=%d)\n",
        data->proto_or_player_id,
        username,
        uuid,
        public_key,
        data->world_seed,
        data->dimension_id,
        data->world_type_id,
        data->packet_delay
    );
    return 1;
}

static void PacketLogin_destroy(const Packet* nonnull packet) {
    const PacketLogin* data = packet->data;

    if (data->username) {
        data->username->destroy(data->username);
    }
    if (data->public_key) {
        data->public_key->destroy(data->public_key);
    }
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketLogin_read(const Connection* nonnull connection) {
    PacketLogin* data = calloc(1, sizeof(PacketLogin));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketLogin_vtable();

    if (!data) return NULL;

    if (
        connection_read_i32(connection, &data->proto_or_player_id) <= 0 ||
        connection_read_str_utf8(connection, &data->username) <= 0 ||
        connection_read_uuid(connection, &data->uuid) <= 0 ||
        connection_read_str_utf8(connection, &data->public_key) <= 0 ||
        connection_read_i64(connection, &data->world_seed) <= 0 ||
        connection_read_i32(connection, &data->dimension_id) <= 0 ||
        connection_read_i32(connection, &data->world_type_id) <= 0 ||
        connection_read_i8(connection, &data->packet_delay) <= 0
    ) {
        PacketLogin_destroy(packet);
        return NULL;
    }
    return packet;
}

static void PacketLogin_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketLogin* data = packet->data;

    connection_write_i32(connection, data->proto_or_player_id);
    connection_write_str_utf8(connection, data->username);
    connection_write_uuid(connection, data->uuid);
    connection_write_str_utf8(connection, data->public_key);
    connection_write_i64(connection, data->world_seed);
    connection_write_i32(connection, data->dimension_id);
    connection_write_i32(connection, data->world_type_id);
    connection_write_i8(connection, data->packet_delay);
}

const Packet_VTable* nonnull PacketLogin_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketLogin_read,
        .write = PacketLogin_write,
        .destroy = PacketLogin_destroy,
        .handle = PacketLogin_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketLogin_factory() {
    static const PacketFactory factory = {
        .read = PacketLogin_read
    };

    return &factory;
}
