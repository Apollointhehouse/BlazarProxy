#include "packet/container/PacketRecipeSync.h"

#include <inttypes.h>

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"

static ssize_t PacketRecipeSync_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketRecipeSync* data = packet->data;

    STRING_UTF8_TO_C_STR(recipe, data->recipe);

    printf("PacketRecipeSync(recipe=\"%s\", max_recipes=%"PRId64")\n",recipe, data->max_recipes);

    return 1;
}

static void PacketRecipeSync_destroy(const Packet* nonnull packet) {
    const PacketRecipeSync* data = packet->data;

    data->recipe->destroy(data->recipe);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketRecipeSync_read(const Connection* nonnull connection) {
    PacketRecipeSync* data = calloc(1, sizeof(PacketRecipeSync));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketRecipeSync_vtable();

    if (
        connection_read_str_utf8(connection, &data->recipe) <= 0 ||
        connection_read_i64(connection, &data->max_recipes) < 0
    ) {
        PacketRecipeSync_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketRecipeSync_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketRecipeSync* data = packet->data;

    connection_write_str_utf8(connection, data->recipe);
    connection_write_i64(connection, data->max_recipes);
}

const Packet_VTable* nonnull PacketRecipeSync_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketRecipeSync_read,
        .write = PacketRecipeSync_write,
        .destroy = PacketRecipeSync_destroy,
        .handle = PacketRecipeSync_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketRecipeSync_factory() {
    static const PacketFactory factory = {
        .read = PacketRecipeSync_read
    };

    return &factory;
}
