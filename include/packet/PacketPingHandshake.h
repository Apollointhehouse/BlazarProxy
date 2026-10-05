#pragma once

#include "packet/PacketPingHandshake.h"
#include <stdint.h>
#include "PacketFactory.h"
#include "util/StringUTF16.h"

typedef struct PacketPingHandshake {
    uint8_t payload;
    uint8_t identifier;
    StringUTF16* ping_host_string;
    uint8_t protocol_version;
    StringUTF16* hostname;
    int32_t port;
} PacketPingHandshake;

const Packet_VTable* PacketPingHandshake_vtable();
const PacketFactory* PacketPingHandshake_factory();