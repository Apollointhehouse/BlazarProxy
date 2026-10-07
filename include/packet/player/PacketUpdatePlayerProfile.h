#pragma once

#include "packet/Packet_VTable.h"
#include "util/UUID.h"

typedef struct PacketUpdatePlayerProfile {
    StringUTF8* nonnull username;
    StringUTF16* nonnull nickname;
    UUID uuid;
    int32_t score;
    int8_t chat_color;
    int8_t is_online;
    int8_t is_operator;
} PacketUpdatePlayerProfile;

const Packet_VTable* nonnull PacketUpdatePlayerProfile_vtable();
const PacketFactory* nonnull PacketUpdatePlayerProfile_factory();