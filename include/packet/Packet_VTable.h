#pragma once

#include <stdio.h>
#include "Connection.h"
#include "ConnectionContext.h"
#include "Packet.h"

typedef struct Packet_VTable {
    const Packet* nullable (*nonnull read)(const Connection* nonnull connection);
    void (*nonnull write)(const Packet* nonnull packet, const Connection* nonnull connection);
    void (*nonnull destroy)(const Packet* nonnull packet);
    ssize_t (*nonnull handle)(const Packet* nonnull packet, const ConnectionContext* nonnull ctx);
} Packet_VTable;