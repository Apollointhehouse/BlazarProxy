#pragma once

#include "../PacketFactory.h"
#include "../Packet_VTable.h"
#include "util/StringUTF16.h"

typedef struct PacketDisconnect {
    StringUTF16* nonnull reason;
} PacketDisconnect;

const Packet_VTable* nonnull PacketDisconnect_vtable();
const PacketFactory* nonnull PacketDisconnect_factory();