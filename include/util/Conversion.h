#pragma once
#include <stdint.h>

typedef union UintToFloat {
    uint32_t u;
    float f;
} UintToFloat;

typedef union UintToDouble {
    uint32_t u;
    double d;
} UintToDouble;
