#include "util/StringUTF16.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static void string_utf16_destroy(StringUTF16* self) {
    if (!self) return;

    if (self->data) {
        free((void*)self->data->buffer);
        self->data->buffer = NULL;
        free(self->data);
    }

    free(self);
}

static size_t string_utf16_to_ascii(const StringUTF16* self, char* dst, const size_t cap) {
    if (!dst || cap == 0) return 0;
    dst[0] = '\0';
    if (!self || !self->data || !self->data->buffer) return 0;

    const uint8_t* src = self->data->buffer;
    const size_t len = self->data->length;

    size_t out = 0;
    for (size_t i = 0; i + 1 < len && out + 1 < cap; i += 2) {
        const uint8_t hi = src[i];
        const uint8_t lo = src[i + 1];

        if (hi == 0x00 && lo == 0xA7) {
            i += 2;
            continue;
        }

        char c;
        if (hi == 0x00 && lo >= 0x20 && lo <= 0x7E) c = (char)lo;
        else if (hi == 0x00 && lo == 0x00) c = '|';
        else c = '?';

        dst[out++] = c;
    }
    dst[out] = '\0';
    return out;
}

static void string_utf16_to_c_string(const StringUTF16* self, char* buffer) {
    string_utf16_to_ascii(self, buffer, self->data->length / 2 + 1);
}

static void string_utf16_print(const StringUTF16* self) {
    const size_t cap = self->data->length / 2 + 1;
    char* tmp = malloc(cap);
    if (!tmp) return;
    string_utf16_to_ascii(self, tmp, cap);
    fputs(tmp, stdout);
    free(tmp);
}

static size_t string_utf16_size(const StringUTF16* self) {
    return self->data->length;
}

static const uint8_t* string_utf16_buffer(const StringUTF16* self) {
    return self->data->buffer;
}

StringUTF16* string_utf16_from_c_str(const char* s, const size_t n) {
    uint8_t* buf = malloc(n * 2 + 1);
    if (!buf) return NULL;
    for (size_t i = 0; i < n; i++) {
        buf[2 * i]     = 0x00;
        buf[2 * i + 1] = (uint8_t)s[i];
    }
    StringUTF16* str = string_utf16_create(buf, n * 2);
    if (!str) free(buf);
    return str;
}

StringUTF16* string_utf16_create(const uint8_t* buffer, const size_t length) {
    StringUTF16* self = malloc(sizeof(StringUTF16));
    if (!self) return NULL;

    self->data = malloc(sizeof(String16BE_Data));
    if (!self->data) return NULL;
    self->data->length = length;
    self->data->buffer = buffer;

    self->destroy = string_utf16_destroy;
    self->print = string_utf16_print;
    self->to_c_string = string_utf16_to_c_string;
    self->size = string_utf16_size;
    self->buffer = string_utf16_buffer;
    return self;
}
