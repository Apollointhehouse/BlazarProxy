#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketHandshake {
    StringUTF8* nonnull username;
} PacketHandshake;

const Packet_VTable* nonnull PacketHandshake_vtable();
const PacketFactory* nonnull PacketHandshake_factory();