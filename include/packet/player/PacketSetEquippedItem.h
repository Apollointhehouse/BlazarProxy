#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSetEquippedItem {
    int32_t entity_id;
    int16_t slot;
    int16_t item_id;
    int16_t item_meta;
    NBT* nonnull item_data;
} PacketSetEquippedItem;

const Packet_VTable* nonnull PacketSetEquippedItem_vtable();
const PacketFactory* nonnull PacketSetEquippedItem_factory();