#include "packet/PacketDisconnect.h"

#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"
#include "util/StringUTF16.h"


static void PacketDisconnect_handle(const void* self) {
    const PacketDisconnect* packet = self;

    STRING_UTF16_TO_C_STR(reason, packet->reason);
    printf("PacketDisconnect(reason=\"%s\")\n", reason);
}

static void PacketDisconnect_destroy(const void* self) {
    const PacketDisconnect* packet = self;
    if (packet->reason) packet->reason->destroy(packet->reason);
    free((void*)packet);
}

static void* PacketDisconnect_read(const Connection* connection) {
    PacketDisconnect* packet = calloc(1, sizeof(PacketDisconnect));

    if (!packet) return NULL;

    if (
        connection_read_str_utf16(connection, &packet->reason) <= 0
    ) {
        perror("failed to read str disconnect packet \n");
        PacketDisconnect_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketDisconnect_write(const void* self, const Connection* connection) {
    const PacketDisconnect* packet = self;

    connection_write_str_utf16(connection, packet->reason);
}

PacketEntry PacketDisconnect_vtable() {
    return (PacketEntry) {
        .read = PacketDisconnect_read,
        .write = PacketDisconnect_write,
        .destroy = PacketDisconnect_destroy,
        .handle = PacketDisconnect_handle,
    };
}