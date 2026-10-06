#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketPlayerConfig {
    int32_t entity_id;
    int16_t config;
} PacketPlayerConfig;

const Packet_VTable* nonnull PacketPlayerConfig_vtable();
const PacketFactory* nonnull PacketPlayerConfig_factory();