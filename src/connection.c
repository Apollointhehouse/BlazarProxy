#include "../include/connection.h"

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

int8_t connection_read_i8(const Connection* self) {
    int8_t bytes[1];
    read(self->socket, bytes, 1);

    return bytes[0];
}

int16_t connection_read_i16(const Connection* self) {
    int8_t bytes[2];
    read(self->socket, bytes, 2);

    return (int16_t)((int16_t)bytes[0] << 8) + bytes[1];
}

int32_t connection_read_i32(const Connection* self) {
    int8_t bytes[4];
    read(self->socket, bytes, 4);

    return ((int32_t)bytes[0] << 24) + ((int32_t)bytes[1] << 16) + ((int32_t)bytes[2] << 8) + bytes[3];
}

int64_t connection_read_i64(const Connection* self) {
    // int8_t bytes[1];
    // read(self->socket, bytes, 1);
    //
    // return bytes[0];
    return 0;
}

StringBE* connection_read_str(const Connection* self) {
    const size_t length = connection_read_i16(self) * 2;

    u_int8_t* buffer = malloc(sizeof(u_int8_t) * length);

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
