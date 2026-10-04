#include "../include/connection.h"

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "util/String16BE.h"

struct Connection {
    int32_t socket;
};

Connection* connection_create(const int32_t new_socket) {
    Connection* args = malloc(sizeof(struct Connection));
    args->socket = new_socket;
    return args;
}

void connection_destroy(Connection* self) {
    close(self->socket);
    free(self);
}

static ssize_t connection_read_bytes_be(const Connection* self, size_t size, void** out) {
    int8_t bytes[size];
    const ssize_t code = read(self->socket, bytes, size);

    for (int i = 0; i < size; i++) {
        *out += bytes[i] << ((size - i - 1) * 8);
    }

    return code;
}

ssize_t connection_read_i8(const Connection* self, int8_t* out) {
    return connection_read_bytes_be(self, sizeof(int8_t), (void**)out);
}

ssize_t connection_read_i16(const Connection* self, int16_t* out) {
    return connection_read_bytes_be(self, sizeof(int16_t), (void**)out);
}

ssize_t connection_read_i32(const Connection* self, int32_t* out) {
    return connection_read_bytes_be(self, sizeof(int32_t), (void**)out);
}

ssize_t connection_read_i64(const Connection* self, int64_t* out) {
    return connection_read_bytes_be(self, sizeof(int64_t), (void**)out);
}

ssize_t connection_read_str_16be(const Connection* self, String16BE** out) {
    int16_t length;

    connection_read_i16(self, &length);
    length *= 2;

    uint8_t* buffer = malloc(sizeof(uint8_t) * length);

    const ssize_t code = read(self->socket, buffer, length);

    *out = string_16be_create(buffer, length);

    return code;
}


ssize_t connection_read(const Connection* self, void *read_buffer, const size_t length) {
    return read(self->socket, read_buffer, length);
}

ssize_t connection_write(const Connection* self, const void* data, const size_t length) {
    return send(self->socket, data, length, 0);
}
