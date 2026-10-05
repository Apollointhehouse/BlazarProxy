#include "util/StringUTF8.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static void string_utf8_destroy(StringUTF8* self) {
    if (!self) return;

    if (self->data) {
        free((void*)self->data->buffer);
        self->data->buffer = NULL;
        free(self->data);
    }

    free(self);
}

static size_t string_utf8_to_ascii(const StringUTF8* self, char* dst, const size_t cap) {
    if (!dst || cap == 0) return 0;
    dst[0] = '\0';
    if (!self || !self->data || !self->data->buffer) return 0;

    const uint8_t* src = self->data->buffer;
    const size_t len = self->data->length;

    size_t out = 0;
    for (size_t i = 0; i < len && out < cap; i++) {
        const uint8_t byte = src[i];

        char c;
        if (byte == 0x00) c = '|';
        else c = (char)byte;

        dst[out++] = c;
    }
    dst[out] = '\0';
    return out;
}

static void string_utf8_to_c_string(const StringUTF8* self, char* buffer) {
    string_utf8_to_ascii(self, buffer, self->data->length + 1);
}

static void string_utf8_print(const StringUTF8* self) {
    const size_t cap = self->data->length + 1;
    char* tmp = malloc(cap);
    if (!tmp) return;
    string_utf8_to_ascii(self, tmp, cap);
    fputs(tmp, stdout);
    free(tmp);
}

static size_t string_utf8_size(const StringUTF8* self) {
    return self->data->length;
}

static const uint8_t* string_utf8_buffer(const StringUTF8* self) {
    return self->data->buffer;
}

StringUTF8* string_utf8_from_c_str(const char* s, const size_t n) {
    uint8_t* buf = malloc(n + 1);
    if (!buf) return NULL;
    for (size_t i = 0; i < n; i++) {
        buf[i] = (uint8_t)s[i];
    }
    StringUTF8* str = string_utf8_create(buf, n);
    if (!str) free(buf);
    return str;
}

StringUTF8* string_utf8_create(const uint8_t* buffer, const size_t length) {
    StringUTF8* self = malloc(sizeof(StringUTF8));
    if (!self) return NULL;

    self->data = malloc(sizeof(StringUTF8));
    if (!self->data) return NULL;
    self->data->length = length;
    self->data->buffer = buffer;

    self->destroy = string_utf8_destroy;
    self->print = string_utf8_print;
    self->to_c_string = string_utf8_to_c_string;
    self->size = string_utf8_size;
    self->buffer = string_utf8_buffer;
    return self;
}
