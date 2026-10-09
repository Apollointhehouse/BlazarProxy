#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketUpdatePlayerState {
    int8_t state;
} PacketUpdatePlayerState;

const Packet_VTable* nonnull PacketUpdatePlayerState_vtable();
const PacketFactory* nonnull PacketUpdatePlayerState_factory();