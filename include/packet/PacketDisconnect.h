#pragma once

#include "PacketFactory.h"
#include "Packet_VTable.h"
#include "util/StringUTF16.h"

typedef struct PacketDisconnect {
    StringUTF16* reason;
} PacketDisconnect;

const Packet_VTable* PacketDisconnect_vtable();
const PacketFactory* PacketDisconnect_factory();