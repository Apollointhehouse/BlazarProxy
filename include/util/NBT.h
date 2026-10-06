#pragma once

#include <stdint.h>
#include "nullability.h"

typedef struct NBT {
    int16_t size;
    uint8_t* buffer;
} NBT;

void NBT_destroy(const NBT* nonnull self);