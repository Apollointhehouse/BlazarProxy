#pragma once

#include <stddef.h>
#include "util/StringUTF16.h"
#include "util/StringUTF8.h"
#include <stdio.h>

#include "nullability.h"
#include "util/UUID.h"

typedef struct Connection {
    int32_t socket;
} Connection;

Connection* nullable connection_create(int32_t new_socket);

void connection_destroy(const Connection* nonnull self);

ssize_t connection_read_i8(const Connection* nonnull self, int8_t* nonnull out);
ssize_t connection_read_i16(const Connection* nonnull self, int16_t* nonnull out);
ssize_t connection_read_i32(const Connection* nonnull self, int32_t* nonnull out);
ssize_t connection_read_i64(const Connection* nonnull self, int64_t* nonnull out);
ssize_t connection_read_str_utf16(const Connection* nonnull self, StringUTF16*nonnull *nonnull out);
ssize_t connection_read_str_utf8(const Connection* nonnull self, StringUTF8*nonnull *nonnull out);
ssize_t connection_read_uuid(const Connection* nonnull self, UUID* nonnull out);

ssize_t connection_write_i8(const Connection* nonnull self, int8_t in);
ssize_t connection_write_i16(const Connection* nonnull self, int16_t in);
ssize_t connection_write_i32(const Connection* nonnull self, int32_t in);
ssize_t connection_write_i64(const Connection* nonnull self, int64_t in);
ssize_t connection_write_str_utf16(const Connection* nonnull self, const StringUTF16* nonnull in);
ssize_t connection_write_str_utf8(const Connection* nonnull self, const StringUTF8* nonnull in);
ssize_t connection_write_uuid(const Connection* nonnull self, UUID in);

ssize_t connection_read(const Connection* nonnull self, void* nonnull read_buffer, size_t length);
ssize_t connection_write(const Connection* nonnull self, const void* nonnull data, size_t length);

void connection_shutdown(const Connection* nonnull con);