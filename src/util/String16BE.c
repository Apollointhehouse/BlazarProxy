#include "../../include/util/String16BE.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


String16BE* string_16be_create(uint8_t* buffer, const size_t length) {
    String16BE* self = malloc(sizeof(String16BE));
    self->length = length;
    self->buffer = buffer;
    return self;
}

void string_16be_destroy(String16BE* self) {
    free(self->buffer);
    self->buffer = NULL;
    free(self);
}

void string_16be_print(const String16BE* self) {
    for (size_t i = 0; i + 1 < self->length; i += 2) {
        putchar(self->buffer[i + 1]);
    }
}

void string_16be_to_c_string(const String16BE* self, char* buffer) {
    size_t out = 0;
    for (size_t i = 0; i + 1 < self->length; i += 2) {
        buffer[out++] = (char)self->buffer[i + 1];
    }
    buffer[out] = '\0';
}