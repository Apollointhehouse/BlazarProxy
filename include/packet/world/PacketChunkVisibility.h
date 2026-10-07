#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketChunkVisibility {
    int32_t chunk_x;
    int32_t chunk_z;
    int8_t player_added;
} PacketChunkVisibility;

const Packet_VTable* nonnull PacketChunkVisibility_vtable();
const PacketFactory* nonnull PacketChunkVisibility_factory();