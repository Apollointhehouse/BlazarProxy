#include "../../include/util/String.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


StringBE* string_create(uint8_t* buffer, const size_t length) {
    StringBE* self = malloc(sizeof(StringBE));
    self->length = length;
    self->buffer = buffer;
    return self;
}


void string_destroy(StringBE* self) {
    free(self->buffer);
    self->buffer = NULL;
    free(self);
}

void string_print(const StringBE* self) {
    for (size_t i = 0; i + 1 < self->length; i += 2) {
        putchar(self->buffer[i + 1]);
    }
}