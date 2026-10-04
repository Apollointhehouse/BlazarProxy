#pragma once
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct StringUTF16_Data {
    size_t length;
    const uint8_t* buffer;
} String16BE_Data;

typedef struct StringUTF16 {
    String16BE_Data* data;
    void (*destroy)(struct StringUTF16* self);
    void (*print)(const struct StringUTF16* self);
    void (*to_c_string)(const struct StringUTF16* self, char* buffer);
    size_t (*size)(const struct StringUTF16* self);
    const u_int8_t* (*buffer)(const struct StringUTF16* self);
} StringUTF16;

StringUTF16* string_utf16_from_c_str(const char* s, size_t n);
StringUTF16* string_utf16_create(const u_int8_t* buffer, size_t length);

#define STRING_UTF16_TO_C_STR(name, str)                  \
    char name[(str)->size(str) / 2 + 1];               \
    str->to_c_string((str), name)