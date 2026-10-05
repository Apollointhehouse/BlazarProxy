#pragma once

#include "packet/Packet_VTable.h"
#include "util/UUID.h"

typedef struct PacketLogin {
    int32_t proto_or_player_id;
    StringUTF8* nonnull username;
    UUID uuid;
    int64_t world_seed;
    int32_t dimension_id;
    int32_t world_type_id;
    int8_t packet_delay;
    StringUTF8* nonnull public_key;
} PacketLogin;

const Packet_VTable* nonnull PacketLogin_vtable();
const PacketFactory* nonnull PacketLogin_factory();