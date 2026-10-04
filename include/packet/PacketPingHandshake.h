#pragma once

#include "packet/PacketPingHandshake.h"

#include <stdint.h>

#include "connection.h"
#include "PacketEntry.h"
#include "util/String16BE.h"

typedef struct PacketPingHandshake {
    uint8_t payload;
    uint8_t identifier;
    String16BE* ping_host_string;
    uint8_t protocol_version;
    String16BE* hostname;
    int32_t port;
} PacketPingHandshake;

PacketEntry PacketPingHandshake_vtable();

void* PacketPingHandshake_read(const Connection* connection);
void PacketPingHandshake_write(const void* self, const Connection* connection);

void PacketPingHandshake_destroy(const void* self);

void PacketPingHandshake_handle(const void* self);