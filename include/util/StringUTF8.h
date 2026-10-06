#pragma once
#include <stddef.h>
#include <stdint.h>
#include "nullability.h"

typedef struct StringUTF8_Data {
    size_t length;
    const uint8_t* nonnull buffer;
} StringUTF8_Data;

typedef struct StringUTF8 {
    StringUTF8_Data* nonnull data;
    void (*nonnull destroy)(struct StringUTF8* nonnull self);
    void (*nonnull print)(const struct StringUTF8* nonnull self);
    void (*nonnull to_c_string)(const struct StringUTF8* nonnull self, char* nonnull buffer);
    size_t (*nonnull size)(const struct StringUTF8* nonnull self);
    const uint8_t* nonnull (*nonnull buffer)(const struct StringUTF8* nonnull self);
} StringUTF8;

StringUTF8* nullable string_utf8_from_c_str(const char* nonnull s, size_t n);
StringUTF8* nullable string_utf8_create(const uint8_t* nonnull buffer, size_t length);

#define STRING_UTF8_TO_C_STR(name, str)                  \
    char name[(str)->size(str) + 1];               \
    str->to_c_string((str), name)

#define BYTES_TO_HEX_STR(str_name, byte_arr, length) \
    char str_name[(length) * 2 + 1]; \
    for (size_t _i = 0; _i < (size_t)(length); _i++) { \
        sprintf(&str_name[_i * 2], "%02X", (byte_arr)[_i]); \
    } \
    str_name[(length) * 2] = '\0'
