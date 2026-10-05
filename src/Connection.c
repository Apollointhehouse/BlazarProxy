#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "util/StringUTF16.h"
#include "Connection.h"
#include "util/StringUTF8.h"

struct Connection {
    int32_t socket;
};

Connection* connection_create(const int32_t new_socket) {
    Connection* args = malloc(sizeof(struct Connection));
    if (!args) return NULL;
    args->socket = new_socket;
    return args;
}

void connection_destroy(const Connection* self) {
    close(self->socket);
    free((void*)self);
}

static ssize_t connection_read_all(const Connection* self, const size_t size, void* out) {
    if (size == 0) return 0;

    uint8_t* dst = out;
    size_t got = 0;

    while (got < size) {
        const ssize_t r = read(self->socket, dst + got, size - got);
        if (r <= 0) return r;
        got += (size_t)r;
    }
    return (ssize_t)got;
}


static ssize_t connection_read_be(const Connection* self, const size_t size, uint64_t* out) {
    if (size == 0 || size > sizeof(uint64_t)) {
        errno = EINVAL;
        return -1;
    }

    uint8_t bytes[sizeof(uint64_t)];
    const ssize_t code = connection_read_all(self, size, bytes);
    if (code <= 0) return code;

    uint64_t value = 0;
    for (size_t i = 0; i < size; i++) {
        value = (value << 8) | bytes[i];
    }

    *out = value;
    return code;
}

static ssize_t connection_write_bytes_be(const Connection* self, const size_t size, const int8_t* in) {
    int8_t bytes[size];

    for (size_t i = 0; i < size; i++) {
        bytes[i] = (int8_t)in[size - 1 - i];
    }

    return write(self->socket, bytes, size);
}

ssize_t connection_read_i8(const Connection* self, int8_t* out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int8_t), &v);
    if (code <= 0) return code;
    *out = (int8_t)v;
    return code;
}

ssize_t connection_read_i16(const Connection* self, int16_t* out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int16_t), &v);
    if (code <= 0) return code;
    *out = (int16_t)v;
    return code;
}

ssize_t connection_read_i32(const Connection* self, int32_t* out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int32_t), &v);
    if (code <= 0) return code;
    *out = (int32_t)v;
    return code;
}

ssize_t connection_read_i64(const Connection* self, int64_t* out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int64_t), &v);
    if (code <= 0) return code;
    *out = (int64_t)v;
    return code;
}

ssize_t connection_read_str_utf16(const Connection* self, StringUTF16** out) {
    int16_t length;
    ssize_t code;

    if ((code = connection_read_i16(self, &length)) <= 0) {
        if (code < 0) {
            perror("Failed to read str 16BE length (Error)");
        } else {
            fprintf(stderr, "Client disconnected while reading string length.\n");
        }
        return code;
    }
    length *= 2;

    if (length <= 0) {
        errno = EINVAL;
        perror("Failed to read string 16BE length (Error)");
        return 0;
    }

    uint8_t* buffer = calloc(length, sizeof(uint8_t));
    if (!buffer) return -1;

    if ((code = connection_read_all(self, length, buffer)) <= 0) {
        if (code < 0) {
            perror("Failed to read str 16BE bytes (Error)");
        } else {
            fprintf(stderr, "Client disconnected while reading string bytes.\n");
        }
        free(buffer);
        return code;
    }

    *out = string_utf16_create(buffer, length);
    return code;
}

ssize_t connection_read_str_utf8(const Connection* self, StringUTF8** out) {
    int16_t length;
    ssize_t code;

    if ((code = connection_read_i16(self, &length)) <= 0) {
        if (code < 0) {
            perror("Failed to read str utf-8 BE length (Error)");
        } else {
            fprintf(stderr, "Client disconnected while reading string length.\n");
        }
        return code;
    }

    if (length <= 0) {
        errno = EINVAL;
        perror("Failed to read string utf-8 BE length (Error)");
        return 0;
    }

    uint8_t* buffer = calloc(length, sizeof(uint8_t));
    if (!buffer) return -1;

    if ((code = connection_read_all(self, length, buffer)) <= 0) {
        if (code < 0) {
            perror("Failed to read str utf-8 BE bytes (Error)");
        } else {
            fprintf(stderr, "Client disconnected while reading string bytes.\n");
        }
        free(buffer);
        return code;
    }

    *out = string_utf8_create(buffer, length);
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

ssize_t connection_write_str_utf16(const Connection* self, const StringUTF16* in) {
    const int16_t length = (int16_t)in->size(in) / 2;

    connection_write_i16(self, length);

    const ssize_t code = write(self->socket, in->buffer(in), length * 2);

    return code;
}

ssize_t connection_write_str_utf8(const Connection* self, const StringUTF8* in) {
    const int16_t length = (int16_t)in->size(in);

    connection_write_i16(self, length);

    const ssize_t code = write(self->socket, in->buffer(in), length);

    return code;
}

ssize_t connection_read(const Connection* self, void *read_buffer, const size_t length) {
    return read(self->socket, read_buffer, length);
}

ssize_t connection_write(const Connection* self, const void* data, const size_t length) {
    return send(self->socket, data, length, 0);
}

void connection_shutdown(const Connection* con) {
    if (con) shutdown(con->socket, SHUT_RDWR);
}