#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketTileEntityData {
    NBT tag;
} PacketTileEntityData;

const Packet_VTable* nonnull PacketTileEntityData_vtable();
const PacketFactory* nonnull PacketTileEntityData_factory();