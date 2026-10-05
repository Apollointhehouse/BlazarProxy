#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct StringUTF8_Data {
    size_t length;
    const uint8_t* buffer;
} StringUTF8_Data;

typedef struct StringUTF8 {
    StringUTF8_Data* data;
    void (*destroy)(struct StringUTF8* self);
    void (*print)(const struct StringUTF8* self);
    void (*to_c_string)(const struct StringUTF8* self, char* buffer);
    size_t (*size)(const struct StringUTF8* self);
    const uint8_t* (*buffer)(const struct StringUTF8* self);
} StringUTF8;

StringUTF8* string_utf8_from_c_str(const char* s, size_t n);
StringUTF8* string_utf8_create(const uint8_t* buffer, size_t length);

#define STRING_UTF8_TO_C_STR(name, str)                  \
    char name[(str)->size(str) / 2 + 1];               \
    str->to_c_string((str), name)
