#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketKeepAlive PacketKeepAlive;

const Packet_VTable* nonnull PacketKeepAlive_vtable();
const PacketFactory* nonnull PacketKeepAlive_factory();