#pragma once

#include "Packet_VTable.h"
#include "packet/Packet.h"

typedef struct PacketFactory {
    const Packet* (*read)(const Connection* connection);
} PacketFactory;