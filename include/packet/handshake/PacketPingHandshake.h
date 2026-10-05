#pragma once

#include "packet/handshake/PacketPingHandshake.h"
#include <stdint.h>
#include "../PacketFactory.h"
#include "util/StringUTF16.h"

typedef struct PacketPingHandshake {
    uint8_t payload;
    uint8_t identifier;
    StringUTF16* nonnull ping_host_string;
    uint8_t protocol_version;
    StringUTF16* nonnull hostname;
    int32_t port;
} PacketPingHandshake;

const Packet_VTable* nonnull PacketPingHandshake_vtable();
const PacketFactory* nonnull PacketPingHandshake_factory();