#include "util/UUID.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "nullability.h"

char* nullable uuid_to_c_str(const UUID uuid, char *nonnull out_str) {
    if (out_str == NULL) {
        return NULL;
    }

    const uint32_t part1 = (uint32_t)(uuid.data[0] >> 32);
    const uint16_t part2 = (uint16_t)(uuid.data[0] >> 16);
    const uint16_t part3 = (uint16_t)uuid.data[0];

    const uint16_t part4 = (uint16_t)(uuid.data[1] >> 48);
    const uint64_t part5 = uuid.data[1] & 0x0000FFFFFFFFFFFFULL;

    snprintf(out_str, UUID_STR_SIZE,
             "%08" PRIx32 "-%04" PRIx16 "-%04" PRIx16 "-%04" PRIx16 "-%012" PRIx64,
             part1, part2, part3, part4, part5);

    return out_str;
}