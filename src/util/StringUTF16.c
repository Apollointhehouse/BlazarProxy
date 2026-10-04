#include "../../include/util/StringUTF16.h"

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

static void string_utf16_print(const StringUTF16* self) {
    for (size_t i = 0; i + 1 < self->data->length; i += 2) {
        putchar(self->data->buffer[i + 1]);
    }
}

static void string_utf16_to_c_string(const StringUTF16* self, char* buffer) {
    if (!buffer) return;
    if (self->data->length <= 0) return;

    size_t out = 0;
    for (size_t i = 0; i + 1 < self->data->length; i += 2) {
        buffer[out++] = (char)self->data->buffer[i + 1];
    }
    buffer[out] = '\0';
}

static size_t string_utf16_size(const StringUTF16* self) {
    return self->data->length;
}

static const u_int8_t* string_utf16_buffer(const StringUTF16* self) {
    return self->data->buffer;
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
