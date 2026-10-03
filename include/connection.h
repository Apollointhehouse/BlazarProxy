#pragma once

#include <stddef.h>

#include <sys/types.h>

#include "util/String.h"

typedef struct Connection Connection;

Connection* connection_create(int32_t new_socket);

void connection_destroy(Connection* self);


ssize_t connection_read_i8(const Connection* self, int8_t* out);
ssize_t connection_read_i16(const Connection* self, int16_t* out);
ssize_t connection_read_i32(const Connection* self, int32_t* out);

StringBE* connection_read_str(const Connection* self);

ssize_t connection_read(const Connection* self, void *read_buffer, size_t length);

ssize_t connection_write(const Connection* self, const void* data, size_t length);