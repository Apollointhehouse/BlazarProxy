#pragma once
#include "packet/Packet_VTable.h"
#include "util/StringUTF8.h"

typedef struct PacketRecipeSync {
    StringUTF8* nonnull recipe;
    int64_t max_recipes;
} PacketRecipeSync;

const Packet_VTable* nonnull PacketRecipeSync_vtable();
const PacketFactory* nonnull PacketRecipeSync_factory();