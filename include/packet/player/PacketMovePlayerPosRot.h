#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketMovePlayerPosRot {
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    int8_t on_ground;
} PacketMovePlayerPosRot;

const Packet_VTable* nonnull PacketMovePlayerPosRot_vtable();
const PacketFactory* nonnull PacketMovePlayerPosRot_factory();