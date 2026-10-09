#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSetHotbarOffset {
    int8_t hotbar_offset;
} PacketSetHotbarOffset;

const Packet_VTable* nonnull PacketSetHotbarOffset_vtable();
const PacketFactory* nonnull PacketSetHotbarOffset_factory();