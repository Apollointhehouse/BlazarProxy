#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSetHeldObject {
    int32_t entity_id;
    NBT* object_tag;
} PacketSetHeldObject;

const Packet_VTable* nonnull PacketSetHeldObject_vtable();
const PacketFactory* nonnull PacketSetHeldObject_factory();