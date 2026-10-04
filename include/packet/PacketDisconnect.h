#pragma once

#include "PacketEntry.h"
#include "util/StringUTF16.h"

typedef struct PacketDisconnect {
    StringUTF16* reason;
} PacketDisconnect;

PacketEntry PacketDisconnect_vtable();