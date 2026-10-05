#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketHandshake {
    StringUTF8* username;
} PacketHandshake;

const Packet_VTable* PacketHandshake_vtable();
const PacketFactory* PacketHandshake_factory();