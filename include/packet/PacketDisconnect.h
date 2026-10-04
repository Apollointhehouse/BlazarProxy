#pragma once

#include "connection.h"
#include "PacketEntry.h"
#include "util/String16BE.h"

typedef struct PacketDisconnect {
    String16BE* reason;
} PacketDisconnect;

PacketEntry PacketDisconnect_vtable();

void* PacketDisconnect_read(const Connection* connection);
void PacketDisconnect_write(const void* self, const Connection* connection);
void PacketDisconnect_destroy(const void* self);
void PacketDisconnect_handle(const void* self);