#include "../include/connection.h"

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include "util/String.h"

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

ssize_t connection_read_i8(const Connection* self, int8_t* out) {
    int8_t bytes[1];
    const ssize_t code = read(self->socket, bytes, 1);

    *out = bytes[0];

    return code;
}

ssize_t connection_read_i16(const Connection* self, int16_t* out) {
    int8_t bytes[2];
    const ssize_t code = read(self->socket, bytes, 2);

    *out = (int16_t)((int16_t)bytes[0] << 8) + bytes[1];

    return code;
}

ssize_t connection_read_i32(const Connection* self, int32_t* out) {
    int8_t bytes[4];
    const ssize_t code = read(self->socket, bytes, 4);

    *out = ((int32_t)bytes[0] << 24) + ((int32_t)bytes[1] << 16) + ((int32_t)bytes[2] << 8) + bytes[3];

    return code;
}

ssize_t connection_read_i64(const Connection* self) {
    // int8_t bytes[1];
    // read(self->socket, bytes, 1);
    //
    // return bytes[0];
    return 0;
}

StringBE* connection_read_str(const Connection* self) {
    int16_t length;

    connection_read_i16(self, &length);
    length *= 2;

    uint8_t* buffer = malloc(sizeof(uint8_t) * length);

    read(self->socket, buffer, length);

    StringBE* str = string_create(buffer, length);

    return str;
}


ssize_t connection_read(const Connection* self, void *read_buffer, const size_t length) {
    return read(self->socket, read_buffer, length);
}

ssize_t connection_write(const Connection* self, const void* data, const size_t length) {
    return send(self->socket, data, length, 0);
}
