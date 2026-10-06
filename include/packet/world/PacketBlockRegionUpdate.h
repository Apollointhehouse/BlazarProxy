#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketBlockRegionUpdate {
    int32_t x_position;
    int16_t y_position;
    int32_t z_position;
    int8_t x_size;
    int8_t y_size;
    int8_t z_size;
    int32_t chunk_size;
    uint8_t* nonnull chunk;
} PacketBlockRegionUpdate;

const Packet_VTable* nonnull PacketBlockRegionUpdate_vtable();
const PacketFactory* nonnull PacketBlockRegionUpdate_factory();