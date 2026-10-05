#pragma once

#include <stddef.h>
#include <stdint.h>
#include "nullability.h"

typedef struct StringUTF16_Data {
    size_t length;
    const uint8_t* nonnull buffer;
} String16BE_Data;

typedef struct StringUTF16 {
    String16BE_Data* nonnull data;
    void (*nonnull destroy)(struct StringUTF16* nonnull self);
    void (*nonnull print)(const struct StringUTF16* nonnull self);
    void (*nonnull to_c_string)(const struct StringUTF16* nonnull self, char* nonnull buffer);
    size_t (*nonnull size)(const struct StringUTF16* nonnull self);
    const uint8_t* nonnull (*nonnull buffer)(const struct StringUTF16* nonnull self);
} StringUTF16;

StringUTF16* nullable string_utf16_from_c_str(const char* nonnull s, size_t n);
StringUTF16* nullable string_utf16_create(const uint8_t* nonnull buffer, size_t length);

#define STRING_UTF16_TO_C_STR(name, str)                  \
    char name[(str)->size(str) / 2 + 1];               \
    str->to_c_string((str), name)
