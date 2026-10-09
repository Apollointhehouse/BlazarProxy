#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketPlayerGamemode {
    int32_t entity_id;
    StringUTF16* nonnull gamemode_id;
} PacketPlayerGamemode;

const Packet_VTable* nonnull PacketPlayerGamemode_vtable();
const PacketFactory* nonnull PacketPlayerGamemode_factory();