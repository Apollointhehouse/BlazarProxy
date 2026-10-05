#include "packet/PacketDisconnect.h"

#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"
#include "packet/PacketFactory.h"
#include "util/StringUTF16.h"


static ssize_t PacketDisconnect_handle(const Packet* packet, const ConnectionContext* ctx) {
    const PacketDisconnect* data = packet->data;

    STRING_UTF16_TO_C_STR(reason, data->reason);
    printf("PacketDisconnect(reason=\"%s\")\n", reason);

    return 1;
}

static void PacketDisconnect_destroy(const Packet* packet) {
    if (!packet) return;

    const PacketDisconnect* data = packet->data;
    if (data->reason) data->reason->destroy(data->reason);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketDisconnect_read(const Connection* connection) {
    PacketDisconnect* data = calloc(1, sizeof(PacketDisconnect));
    Packet* packet = malloc(sizeof(Packet));
    packet->data = data;
    packet->v = PacketDisconnect_vtable();

    if (!data) return NULL;

    if (
        connection_read_str_utf16(connection, &data->reason) <= 0
    ) {
        perror("failed to read str disconnect packet \n");
        PacketDisconnect_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketDisconnect_write(const Packet* packet, const Connection* connection) {
    const PacketDisconnect* data = packet->data;

    connection_write_str_utf16(connection, data->reason);
}

const Packet_VTable* PacketDisconnect_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketDisconnect_read,
        .write = PacketDisconnect_write,
        .destroy = PacketDisconnect_destroy,
        .handle = PacketDisconnect_handle,
    };

    return &vtable;
}

const PacketFactory* PacketDisconnect_factory() {
    static const PacketFactory factory = {
        .read = PacketDisconnect_read
    };

    return &factory;
}
