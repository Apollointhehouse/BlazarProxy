#pragma once

#include "Packet_VTable.h"
#include "packet/Packet.h"

typedef struct PacketFactory {
    const Packet* nullable (*nonnull read)(const Connection* nonnull connection);
} PacketFactory;