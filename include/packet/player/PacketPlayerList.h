#pragma once

#include "packet/Packet_VTable.h"
#include "util/UUID.h"

typedef struct PacketPlayerList {
    int32_t count;
    StringUTF16*nonnull *nonnull players;
    StringUTF16*nonnull *nonnull scores;
} PacketPlayerList;

const Packet_VTable* nonnull PacketPlayerList_vtable();
const PacketFactory* nonnull PacketPlayerList_factory();