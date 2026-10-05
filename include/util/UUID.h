#pragma once
#include <stdint.h>
#include "nullability.h"

#define UUID_STR_SIZE 37

typedef struct UUID {
    uint64_t data[2];
} UUID;

char* uuid_to_c_str(UUID uuid, char* nonnull out_str);

#define UUID_TO_C_STR(name, uuid)                  \
    char name[UUID_STR_SIZE];                      \
    uuid_to_c_str((uuid), name)