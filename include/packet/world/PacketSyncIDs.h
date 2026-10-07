#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSyncIDs {
    int8_t destination_id;
    int32_t mapping_size;
    StringUTF8*nullable *nonnull mapping;
} PacketSyncIDs;

const Packet_VTable* nonnull PacketSyncIDs_vtable();
const PacketFactory* nonnull PacketSyncIDs_factory();