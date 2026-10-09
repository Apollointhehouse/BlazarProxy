#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketRemoveEntity {
    int32_t entity_id;
} PacketRemoveEntity;

const Packet_VTable* nonnull PacketRemoveEntity_vtable();
const PacketFactory* nonnull PacketRemoveEntity_factory();