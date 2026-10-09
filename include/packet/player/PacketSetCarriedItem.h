#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSetCarriedItem {
    int16_t id;
} PacketSetCarriedItem;

const Packet_VTable* nonnull PacketSetCarriedItem_vtable();
const PacketFactory* nonnull PacketSetCarriedItem_factory();