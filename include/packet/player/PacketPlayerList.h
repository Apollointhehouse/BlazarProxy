#pragma once

#include "packet/Packet_VTable.h"
#include "util/UUID.h"

typedef struct PacketPlayerList {
    int32_t count;
    StringUTF16** players;
    StringUTF16** scores;
} PacketPlayerList;

const Packet_VTable* nonnull PacketPlayerList_vtable();
const PacketFactory* nonnull PacketPlayerList_factory();