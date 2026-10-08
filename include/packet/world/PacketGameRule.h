#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketGameRule {
    NBT* tag;
} PacketGameRule;

const Packet_VTable* nonnull PacketGameRule_vtable();
const PacketFactory* nonnull PacketGameRule_factory();