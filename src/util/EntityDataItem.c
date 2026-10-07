#include "util/EntityDataItem.h"
#include "util/DynamicArray.h"
#include <stdlib.h>
#include "Connection.h"

DEFINE_DYNAMIC_ARRAY(EntityDataItem, EntityDataItem*)

EntityDataItem* EntityDataItem_create(
    const int8_t type,
    const int8_t id,
    void* value
) {
    EntityDataItem* item = malloc(sizeof(EntityDataItem));
    item->type = type;
    item->id = id;
    item->value = value;
    return item;
}

void EntityDataItem_destroy(EntityDataItem* self) {
    if (self) free(self);
}

void ItemStack_destroy(ItemStack* self) {
    if (!self) return;
    if (self->item_nbt) NBT_destroy(self->item_nbt);

    free(self);
}

void ChunkCoordinates_destroy(ChunkCoordinates* self) {
    free(self);
}

ssize_t EntityData_pack(const Connection* connection, const int16_t size, EntityDataItem** items) {
    if (items != NULL) {
        for (int i = 0; i < size; i++) {
            const EntityDataItem* item = items[i];

            EntityDataItem_write(connection, item);
        }
    }

    connection_write_i8(connection, (int8_t)0xFF);

    return 1;
}

ssize_t EntityData_unpack(const Connection* connection, DynamicArray_EntityDataItem** out) {
    DynamicArray_EntityDataItem* array = DynamicArray_EntityDataItem_create(1);

    while (1) {
        int8_t id;
        int8_t type;
        int8_t metadata;

        if (connection_read_i8(connection, &id) <= 0) goto fail;

        if (id == (int8_t)0xFF) break;

        if (connection_read_i8(connection, &type) <= 0) goto fail;
        if (connection_read_i8(connection, &metadata) <= 0) goto fail;

        EntityDataItem* item = malloc(sizeof(EntityDataItem));
        if (!item) goto fail;

        item->type = type;
        item->id = id;
        item->value = NULL;

        if ((metadata & 1) == 0) {
            free(item);
            item = EntityDataItem_unpack(connection, type, id);
        }

        DynamicArray_EntityDataItem_append(array, item);
    }

    *out = array;

    return 1;

    fail:
    for (int i = 0; i < array->size; i++) {
        if (array->data[i]) free(array->data[i]);
    }
    DynamicArray_EntityDataItem_destroy(array);
    return -1;
}

static void EntityDataItem_write(const Connection* connection, const EntityDataItem* item) {
    connection_write_i8(connection, (int8_t)(item->id & 0xFF));
    connection_write_i8(connection, (int8_t)(item->type & 0xFF));

    if (!item->value) {
        connection_write_i8(connection, 1);
        return;
    }

    EntityDataItem_pack(connection, item);
}

static void EntityDataItem_pack(
    const Connection* connection,
    const EntityDataItem* item
) {
    const int8_t metadata = 0;
    void* value = item->value;
    const int8_t type = item->type;

    switch (type) {
        case 0: {
            connection_write_i8(connection, metadata);
            connection_write_i8(connection, *(int8_t*)value);
            // free(value);
            break;
        }
        case 1: {
            connection_write_i8(connection, metadata);
            connection_write_i16(connection, *(int16_t*)value);
            // free(value);
            break;
        }
        case 2: {
            connection_write_i8(connection, metadata);
            connection_write_i32(connection, *(int32_t*)value);
            // free(value);
            break;
        }
        case 3: {
            connection_write_i8(connection, metadata);
            connection_write_float(connection, *(float*)value);
            // free(value);
            break;
        }
        case 4: {
            connection_write_i8(connection, metadata);
            connection_write_str_utf8(connection, value);
            // ((StringUTF8*)value)->destroy(value);
            break;
        }
        case 5: {
            connection_write_i8(connection, metadata);
            connection_write_i16(connection, ((ItemStack*)value)->item_id);
            connection_write_i8(connection, ((ItemStack*)value)->item_count);
            connection_write_i16(connection, ((ItemStack*)value)->item_meta);
            connection_write_nbt(connection, ((ItemStack*)value)->item_nbt);
            // ItemStack_destroy(value);
            break;
        }
        case 6: {
            connection_write_i8(connection, metadata);
            connection_write_i32(connection, ((ChunkCoordinates*)value)->x);
            connection_write_i32(connection, ((ChunkCoordinates*)value)->y);
            connection_write_i32(connection, ((ChunkCoordinates*)value)->z);
            // free(value);
            break;
        }
        case 7: {
            connection_write_i8(connection, metadata);
            connection_write_i64(connection, (int64_t)((UUID*)value)->data[0]);
            connection_write_i64(connection, (int64_t)((UUID*)value)->data[1]);
            // free(value);
            break;
        }
        default: {}
    }
}

static EntityDataItem* EntityDataItem_unpack(
    const Connection* connection,
    const int8_t type,
    const int8_t id
) {
    switch (type) {
        case 0: {
            int8_t* value = malloc(sizeof(int8_t));
            connection_read_i8(connection, value);
            return EntityDataItem_create(type, id, value);
        }
        case 1: {
            int16_t* value = malloc(sizeof(int16_t));
            connection_read_i16(connection, value);
            return EntityDataItem_create(type, id, value); break;
        }
        case 2: {
            int32_t* value = malloc(sizeof(int32_t));
            connection_read_i32(connection, value);
            return EntityDataItem_create(type, id, value); break;
        }
        case 3: {
            float* value = malloc(sizeof(float));
            connection_read_float(connection, value);
            return EntityDataItem_create(type, id, value); break;
        }
        case 4: {
            StringUTF8* str;
            connection_read_str_utf8(connection, &str);
            return EntityDataItem_create(type, id, str); break;
        }
        case 5: {
            int16_t item_id;
            int8_t item_count;
            int16_t item_meta;
            NBT* tag = malloc(sizeof(NBT));

            connection_read_i16(connection, &item_id);
            connection_read_i8(connection, &item_count);
            connection_read_i16(connection, &item_meta);
            connection_read_nbt(connection, tag);

            ItemStack* stack = malloc(sizeof(ItemStack));
            stack->item_id = item_id;
            stack->item_count = item_count;
            stack->item_meta = item_meta;
            stack->item_nbt = tag;

            return EntityDataItem_create(type, id, stack);
        }

        case 6: {
            int32_t x;
            int32_t y;
            int32_t z;

            connection_read_i32(connection, &x);
            connection_read_i32(connection, &y);
            connection_read_i32(connection, &z);

            ChunkCoordinates* chunk_coords = malloc(sizeof(ChunkCoordinates));
            chunk_coords->x = x;
            chunk_coords->y = y;
            chunk_coords->z = z;

            return EntityDataItem_create(type, id, chunk_coords);
        }

        case 7: {
            UUID* uuid = malloc(sizeof(UUID));
            connection_read_uuid(connection, uuid);
            return EntityDataItem_create(type, id, uuid);
        }
        default: {
            return EntityDataItem_create(type, id, NULL);
        }
    }

    return EntityDataItem_create(type, id, NULL);
}