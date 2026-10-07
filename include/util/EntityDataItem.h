#pragma once
#include <stdlib.h>

#include "Connection.h"
#include "util/DynamicArray.h"

typedef struct EntityDataItem {
    int8_t type;
    int8_t id;
    void *value;
} EntityDataItem;

DECLARE_DYNAMIC_ARRAY(EntityDataItem, EntityDataItem*)

EntityDataItem* EntityDataItem_create(int8_t type, int8_t id, void* value);

void EntityDataItem_destroy(EntityDataItem* self);

typedef struct ItemStack {
    int16_t item_id;
    int8_t item_count;
    int16_t item_meta;
    NBT* item_nbt;
} ItemStack;

void ItemStack_destroy(ItemStack* self);

typedef struct ChunkCoordinates {
    int32_t x;
    int32_t y;
    int32_t z;
} ChunkCoordinates;

void ChunkCoordinates_destroy(ChunkCoordinates* self);

static void EntityDataItem_write(const Connection* connection, const EntityDataItem* item);
static void EntityDataItem_pack(const Connection* connection, const EntityDataItem* item);
static EntityDataItem* EntityDataItem_unpack(const Connection* connection, int8_t type, int8_t id);

ssize_t EntityData_pack(const Connection* connection, int16_t size, EntityDataItem** items);
ssize_t EntityData_unpack(const Connection* connection, DynamicArray_EntityDataItem** out);