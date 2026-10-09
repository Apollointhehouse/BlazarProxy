#pragma once
#include "packet/Packet_VTable.h"
#include "util/EntityDataItem.h"

typedef struct PacketSetEntityData {
    int32_t entity_id;
    DynamicArray_EntityDataItem* nonnull unpacked_data;
} PacketSetEntityData;

const Packet_VTable* nonnull PacketSetEntityData_vtable();
const PacketFactory* nonnull PacketSetEntityData_factory();