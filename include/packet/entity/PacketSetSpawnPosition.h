#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSetSpawnPosition {
    int32_t x;
    int32_t y;
    int32_t z;
} PacketSetSpawnPosition;

const Packet_VTable* nonnull PacketSetSpawnPosition_vtable();
const PacketFactory* nonnull PacketSetSpawnPosition_factory();