#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketCommandManager {
    StringUTF8* nonnull suggestions;
} PacketCommandManager;

const Packet_VTable* nonnull PacketCommandManager_vtable();
const PacketFactory* nonnull PacketCommandManager_factory();