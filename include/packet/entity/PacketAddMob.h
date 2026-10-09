#pragma once
#include "packet/Packet_VTable.h"
#include "util/EntityDataItem.h"

typedef struct PacketAddMob {
    int32_t id;
    int16_t type;
    int32_t x;
    int32_t y;
    int32_t z;
    int8_t yaw;
    int8_t pitch;
    DynamicArray_EntityDataItem* nonnull unpacked_data;
    StringUTF8* nonnull nickname;
    int8_t chat_color;
} PacketAddMob;

const Packet_VTable* nonnull PacketAddMob_vtable();
const PacketFactory* nonnull PacketAddMob_factory();