#pragma once
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct String {
    size_t length;
    u_int8_t* buffer;
} StringBE;

StringBE* string_create(u_int8_t* buffer, size_t length);
void string_destroy(StringBE* self);

void string_print(const StringBE* self);