#pragma once
#include "packet/Packet_VTable.h"
#include "util/StringUTF8.h"

typedef struct PacketCustomPayload {
    StringUTF8* nonnull net_channel;
    int32_t size;
    uint8_t* data;
} PacketCustomPayload;

const Packet_VTable* nonnull PacketCustomPayload_vtable();
const PacketFactory* nonnull PacketCustomPayload_factory();