#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "util/StringUTF16.h"
#include "Connection.h"

#include "util/logging.h"
#include "util/StringUTF8.h"
#include "util/UUID.h"

Connection* nullable connection_create(const int32_t new_socket) {
    Connection* args = malloc(sizeof(Connection));
    if (!args) return NULL;
    args->socket = new_socket;
    return args;
}

void connection_destroy(const Connection* nonnull self) {
    close(self->socket);
    free((void*)self);
}

ssize_t connection_read(const Connection* nonnull self, const size_t size, void* nonnull out) {
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

static ssize_t connection_read_be(const Connection* nonnull self, const size_t size, uint64_t* nonnull out) {
    uint8_t bytes[sizeof(uint64_t)];
    const ssize_t code = connection_read(self, size, bytes);
    if (code <= 0) return code;

    uint64_t value = 0;
    for (size_t i = 0; i < size; i++) {
        value = (value << 8) | bytes[i];
    }

    *out = value;
    return code;
}

ssize_t connection_write(const Connection* nonnull self, const size_t size, const void* data) {
    return send(self->socket, data, size, 0);
}

static ssize_t connection_write_bytes_be(const Connection* nonnull self, const size_t size, const uint64_t value) {
    uint8_t bytes[sizeof(uint64_t)];
    for (size_t i = 0; i < size; i++) {
        bytes[i] = (uint8_t)(value >> (8 * (size - 1 - i)));
    }

    size_t total_written = 0;
    while (total_written < size) {
        const ssize_t written = write(self->socket, bytes + total_written, size - total_written);

        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        total_written += (size_t)written;
    }

    return (ssize_t)total_written;
}

ssize_t connection_read_i8(const Connection* nonnull self, int8_t* nonnull out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int8_t), &v);
    if (code <= 0) return code;
    *out = (int8_t)v;
    return code;
}

ssize_t connection_read_i16(const Connection* nonnull self, int16_t* nonnull out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int16_t), &v);
    if (code <= 0) return code;
    *out = (int16_t)v;
    return code;
}

ssize_t connection_read_i32(const Connection* nonnull self, int32_t* nonnull out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int32_t), &v);
    if (code <= 0) return code;
    *out = (int32_t)v;
    return code;
}

ssize_t connection_read_i64(const Connection* nonnull self, int64_t* nonnull out) {
    uint64_t v;
    const ssize_t code = connection_read_be(self, sizeof(int64_t), &v);
    if (code <= 0) return code;
    *out = (int64_t)v;
    return code;
}

ssize_t connection_read_str_utf16(const Connection* nonnull self, StringUTF16*nonnull *nonnull out) {
    int16_t length;
    ssize_t code;

    if ((code = connection_read_i16(self, &length)) <= 0) {
        if (code < 0) {
            perror("Failed to read str 16BE length (Error)");
        } else {
            LOG_ERROR("Client disconnected while reading string length.");
        }
        return code;
    }
    length *= 2;

    if (length < 0) {
        errno = EINVAL;
        LOG_SYS_ERROR("Failed to read string 16BE length (Error)");
        return 0;
    }

    if (length == 0) {
        *out = string_utf16_create(calloc(0,0), 0);
        return code;
    }

    uint8_t* buffer = calloc(length, sizeof(uint8_t));
    if (!buffer) return -1;

    if ((code = connection_read(self, length, buffer)) <= 0) {
        if (code < 0) {
            LOG_SYS_ERROR("Failed to read str 16BE bytes (Error)");
        } else {
            LOG_ERROR("Client disconnected while reading string bytes.");
        }
        free(buffer);
        return code;
    }

    *out = string_utf16_create(buffer, length);

    return code + 2;
}

ssize_t connection_read_str_utf8(const Connection* nonnull self, StringUTF8*nonnull *nonnull out) {
    int16_t length;
    ssize_t code;

    if ((code = connection_read_i16(self, &length)) <= 0) {
        return code;
    }

    if (length < 0) {
        errno = EINVAL;
        LOG_SYS_ERROR("Failed to read string utf-8 BE length (Invalid negative size)");
        return -1;
    }

    if (length == 0) {
        *out = string_utf8_create(calloc(0, 0), 0);
        return code;
    }

    uint8_t* buffer = calloc(length, sizeof(uint8_t));
    if (!buffer) return -1;

    if ((code = connection_read(self, length, buffer)) <= 0) {
        free(buffer);
        return code;
    }

    *out = string_utf8_create(buffer, length);

    return code + 2;
}

ssize_t connection_read_uuid(const Connection* nonnull self, UUID* out) {
    int64_t msb;
    int64_t lsb;
    ssize_t code;

    if ((code = connection_read_i64(self, &msb)) <= 0) return code;
    if ((code = connection_read_i64(self, &lsb)) <= 0) return code;

    out->data[0] = (uint64_t)msb;
    out->data[1] = (uint64_t)lsb;

    return code;
}

ssize_t connection_write_i8(const Connection* nonnull self, const int8_t in) {
    return connection_write_bytes_be(self, sizeof(int8_t), in);
}

ssize_t connection_write_i16(const Connection* nonnull self, const int16_t in) {
    return connection_write_bytes_be(self, sizeof(int16_t), in);
}

ssize_t connection_write_i32(const Connection* nonnull self, const int32_t in) {
    return connection_write_bytes_be(self, sizeof(int32_t), in);
}

ssize_t connection_write_i64(const Connection* nonnull self, const int64_t in) {
    return connection_write_bytes_be(self, sizeof(int64_t), in);
}

ssize_t connection_write_str_utf16(const Connection* nonnull self, const StringUTF16* nonnull in) {
    const int16_t length = (int16_t)in->size(in) / 2;

    connection_write_i16(self, length);

    const ssize_t code = write(self->socket, in->buffer(in), length * 2);

    return code;
}

ssize_t connection_write_str_utf8(const Connection* nonnull self, const StringUTF8* in) {
    const int16_t length = (int16_t)in->size(in);

    connection_write_i16(self, length);

    const ssize_t code = write(self->socket, in->buffer(in), length);

    return code;
}

ssize_t connection_write_uuid(const Connection* nonnull self, const UUID in) {
    const uint64_t msb = in.data[0];
    const uint64_t lsb = in.data[1];
    ssize_t code;

    if ((code = connection_write_i64(self, (int64_t)msb)) <= 0) return code;
    if ((code = connection_write_i64(self, (int64_t)lsb)) <= 0) return code;

    return code;
}

void connection_shutdown(const Connection* nonnull con) {
    if (con) shutdown(con->socket, SHUT_RDWR);
}