#pragma once

#include <stdio.h>
#include "Connection.h"
#include "ConnectionContext.h"
#include "Packet.h"

typedef struct Packet_VTable {
    const Packet* (*read)(const Connection* connection);
    void (*write)(const Packet* packet, const Connection* connection);
    void (*destroy)(const Packet* packet);
    ssize_t (*handle)(const Packet* packet, const ConnectionContext* ctx);
} Packet_VTable;