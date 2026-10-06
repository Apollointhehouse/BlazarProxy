#pragma once
#include "packet/Packet_VTable.h"
#include "util/StringUTF8.h"

typedef struct PacketAESSendKey {
    StringUTF8* nonnull key;
} PacketAESSendKey;


const Packet_VTable* nonnull PacketAESSendKey_vtable();
const PacketFactory* nonnull PacketAESSendKey_factory();