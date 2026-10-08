#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketPhotoMode {
    int8_t disabled;
} PacketPhotoMode;

const Packet_VTable* nonnull PacketPhotoMode_vtable();
const PacketFactory* nonnull PacketPhotoMode_factory();