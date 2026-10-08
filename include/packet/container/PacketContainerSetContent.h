#pragma once
#include "packet/Packet_VTable.h"
#include "util/EntityDataItem.h"
#include "util/StringUTF8.h"

typedef struct   {
    int8_t window_id;
    int32_t state_id;
    ItemStack* nullable carried_item;
    int16_t list_size;
    ItemStack*nullable *nonnull stack_list;
} PacketContainerSetContent;

const Packet_VTable* nonnull PacketContainerSetContent_vtable();
const PacketFactory* nonnull PacketContainerSetContent_factory();