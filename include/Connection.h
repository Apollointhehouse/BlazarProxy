#pragma once

#include <stddef.h>

#include <sys/types.h>

#include "util/StringUTF16.h"

typedef struct Connection Connection;

Connection* connection_create(int32_t new_socket);

void connection_destroy(const Connection* self);

ssize_t connection_read_i8(const Connection* self, int8_t* out);
ssize_t connection_read_i16(const Connection* self, int16_t* out);
ssize_t connection_read_i32(const Connection* self, int32_t* out);
ssize_t connection_read_str_utf16(const Connection* self, StringUTF16** out);

ssize_t connection_write_i8(const Connection* self, int8_t in);
ssize_t connection_write_i16(const Connection* self, int16_t in);
ssize_t connection_write_i32(const Connection* self, int32_t in);
ssize_t connection_write_str_utf16(const Connection* self, const StringUTF16* in);

ssize_t connection_read(const Connection* self, void *read_buffer, size_t length);
ssize_t connection_write(const Connection* self, const void* data, size_t length);

void connection_shutdown(const Connection* con);