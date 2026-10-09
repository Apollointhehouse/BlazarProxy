#pragma once

#include <stdint.h>
#include "nullability.h"

typedef struct NBT {
    uint16_t size;
    uint8_t* nullable buffer;
} NBT;

void NBT_destroy(const NBT* nonnull self);