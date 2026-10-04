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

void connection_destroy(const Connection* self) {
    close(self->socket);
    free((void*)self);
}

static ssize_t connection_read_bytes_be(const Connection* self, const size_t size, void* out) {
    uint8_t buf[8];
    if (size == 0 || size > sizeof(buf)) return -1;

    size_t got = 0;
    while (got < size) {
        const ssize_t r = read(self->socket, buf + got, size - got);
        if (r <= 0) return r;
        got += (size_t)r;
    }

    uint8_t* dst = out;
    for (size_t i = 0; i < size; i++) {
        dst[i] = buf[size - 1 - i];
    }

    return (ssize_t)got;
}

static ssize_t connection_write_bytes_be(const Connection* self, const size_t size, const int8_t* in) {
    int8_t bytes[size];

    for (size_t i = 0; i < size; i++) {
        bytes[i] = (int8_t)in[size - 1 - i];
    }

    return write(self->socket, bytes, size);
}

ssize_t connection_read_i8(const Connection* self, int8_t* out) {
    return connection_read_bytes_be(self, sizeof(int8_t), out);
}

ssize_t connection_read_i16(const Connection* self, int16_t* out) {
    return connection_read_bytes_be(self, sizeof(int16_t), out);
}

ssize_t connection_read_i32(const Connection* self, int32_t* out) {
    return connection_read_bytes_be(self, sizeof(int32_t), out);
}

ssize_t connection_read_i64(const Connection* self, int64_t* out) {
    return connection_read_bytes_be(self, sizeof(int64_t), out);
}

ssize_t connection_read_str_16be(const Connection* self, String16BE** out) {
    int16_t length;

    connection_read_i16(self, &length);
    length *= 2;

    uint8_t* buffer = calloc(length, sizeof(uint8_t));

    const ssize_t code = read(self->socket, buffer, length);

    *out = string_16be_create(buffer, length);

    return code;
}

ssize_t connection_write_i8(const Connection* self, const int8_t in) {
    return connection_write_bytes_be(self, sizeof(int8_t), &in);
}

ssize_t connection_write_i16(const Connection* self, const int16_t in) {
    return connection_write_bytes_be(self, sizeof(int16_t), (const int8_t *)&in);
}

ssize_t connection_write_i32(const Connection* self, const int32_t in) {
    return connection_write_bytes_be(self, sizeof(int32_t), (const int8_t *)&in);
}

ssize_t connection_write_str_16be(const Connection* self, const String16BE* in) {
    const int16_t length = (int16_t)in->length / 2;

    connection_write_i16(self, length);

    const ssize_t code = write(self->socket, in->buffer, length * 2);

    return code;
}

ssize_t connection_read(const Connection* self, void *read_buffer, const size_t length) {
    return read(self->socket, read_buffer, length);
}

ssize_t connection_write(const Connection* self, const void* data, const size_t length) {
    return send(self->socket, data, length, 0);
}
