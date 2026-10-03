#pragma once

#include "packet/PacketPingHandshake.h"

#include <stdint.h>

#include "connection.h"
#include "util/String.h"

typedef struct PacketPingHandshake {
    uint8_t payload;
    uint8_t identifier;
    StringBE* ping_host_string;
    uint8_t protocol_version;
    StringBE* hostname;
    int32_t port;
} PacketPingHandshake;

void PacketPingHandshake_write(void* self, const Connection* connection);
void* PacketPingHandshake_create(const Connection* connection);

void PacketPingHandshake_destroy(void* self);

void PacketPingHandshake_handle(const void* self);