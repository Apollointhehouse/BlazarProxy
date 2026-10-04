#pragma once
#include <stddef.h>
#include <sys/types.h>

typedef struct String16BE {
    size_t length;
    u_int8_t* buffer;
} String16BE;

String16BE* string_16be_create(u_int8_t* buffer, size_t length);
void string_16be_destroy(String16BE* self);

void string_16be_print(const String16BE* self);
void string_16be_to_c_string(const String16BE* self, char* buffer);

#define STRING_16BE_TO_C_STR(name, str)                  \
    char name[(str)->length / 2 + 1];               \
    string_16be_to_c_string((str), name)