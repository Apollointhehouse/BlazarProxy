#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketSetTime {
    int64_t time;
} PacketSetTime;

const Packet_VTable* nonnull PacketSetTime_vtable();
const PacketFactory* nonnull PacketSetTime_factory();