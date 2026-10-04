#include "packet/PacketDisconnect.h"

#include <stdio.h>
#include <stdlib.h>

#include "connection.h"
#include "util/String16BE.h"

PacketEntry PacketDisconnect_vtable() {
    return (PacketEntry) {
        .read = PacketDisconnect_read,
        .write = PacketDisconnect_write,
        .destroy = PacketDisconnect_destroy,
        .handle = PacketDisconnect_handle,
    };
}

void* PacketDisconnect_read(const Connection* connection) {
    PacketDisconnect* packet = malloc(sizeof(PacketDisconnect));

    if (!packet) return NULL;

    if (
        connection_read_str_16be(connection, &packet->reason) <= 0
    ) {
        PacketDisconnect_destroy(packet);
        return NULL;
    }

    return packet;
}

void PacketDisconnect_write(const void* self, const Connection* connection) {
    const PacketDisconnect* packet = self;

    connection_write_str_16be(connection, packet->reason);
}

void PacketDisconnect_destroy(const void* self) {
    const PacketDisconnect* packet = self;
    if (packet->reason) string_16be_destroy(packet->reason);
    free((void*)packet);
}

void PacketDisconnect_handle(const void* self) {
    const PacketDisconnect* packet = self;

    STRING_16BE_TO_C_STR(reason, packet->reason);

    printf("disconnect reason: %s\n", reason);
}
