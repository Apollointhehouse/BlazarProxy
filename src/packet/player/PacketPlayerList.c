#include "packet/player/PacketPlayerList.h"

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"

static ssize_t PacketPlayerList_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketPlayerList* data = packet->data;

    printf(
        "PacketPlayerList(count=%d)\n",
        data->count
    );
    return 1;
}

static void PacketPlayerList_destroy(const Packet* nonnull packet) {
    const PacketPlayerList* data = packet->data;

    for (int i = 0; i < data->count; i++) {
        StringUTF16* player = data->players[i];
        StringUTF16* score = data->scores[i];

        player->destroy(player);
        score->destroy(score);
    }

    free(data->players);
    free(data->scores);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketPlayerList_read(const Connection* nonnull connection) {
    PacketPlayerList* data = calloc(1, sizeof(PacketPlayerList));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) {
        free(data);
        return NULL;
    }
    packet->data = data;
    packet->v = PacketPlayerList_vtable();

    if (connection_read_i32(connection, &data->count) <= 0) {
        PacketPlayerList_destroy(packet);
        return NULL;
    }

    if (data->count <= 0) return packet;

    data->players = calloc(data->count, sizeof(StringUTF16*));
    data->scores = calloc(data->count, sizeof(StringUTF16*));

    for (int i = 0; i < data->count; i++) {
        if (
            connection_read_str_utf16(connection, &data->players[i]) < 0 ||
            connection_read_str_utf16(connection, &data->scores[i]) < 0
        ) {
            PacketPlayerList_destroy(packet);
            return NULL;
        }
    }

    return packet;
}

static void PacketPlayerList_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketPlayerList* data = packet->data;

    connection_write_i32(connection, data->count);

    if (data->count <= 0) return;
    if (!data->players || !data->scores) return;

    for (int i = 0; i < data->count; i++) {
        StringUTF16* player = data->players[i];
        StringUTF16* score = data->scores[i];

        if (!player || !score) continue;

        connection_write_str_utf16(connection, player);
        connection_write_str_utf16(connection, score);
    }
}

const Packet_VTable* nonnull PacketPlayerList_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketPlayerList_read,
        .write = PacketPlayerList_write,
        .destroy = PacketPlayerList_destroy,
        .handle = PacketPlayerList_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketPlayerList_factory() {
    static const PacketFactory factory = {
        .read = PacketPlayerList_read
    };

    return &factory;
}
