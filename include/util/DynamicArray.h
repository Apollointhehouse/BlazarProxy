#pragma once

#include "util/EntityDataItem.h"
#include "util/DynamicArray.h"
#include <errno.h>
#include <stdlib.h>
#include "util/logging.h"

#define DECLARE_DYNAMIC_ARRAY(name, T) typedef struct DynamicArray_##name { \
    size_t size; \
    size_t capacity; \
    T* data; \
} DynamicArray_##name; \
 \
T DynamicArray_##name##_get(const DynamicArray_##name* array, size_t index); \
void DynamicArray_##name##_grow(DynamicArray_##name* array); \
void DynamicArray_##name##_append(DynamicArray_##name* array, T value); \
void DynamicArray_##name##_destroy(DynamicArray_##name* array); \
DynamicArray_##name* DynamicArray_##name##_create(size_t capacity); \


#define DEFINE_DYNAMIC_ARRAY(name, T) \
 \
DynamicArray_##name* DynamicArray_##name##_create(const size_t capacity) { \
    DynamicArray_##name* array = calloc(1, sizeof(DynamicArray_##name)); \
 \
    array->size = 0; \
    array->capacity = capacity; \
    array->data = calloc(1, capacity * sizeof(T)); \
 \
    return array; \
} \
 \
T DynamicArray_##name##_get(const DynamicArray_##name* array, const size_t index) { \
    if (index >= array->size) { \
        LOG_ERROR("Index out of bounds %d for size %d", index, array->size); \
        exit(EXIT_FAILURE); \
    } \
 \
    return array->data[index]; \
} \
 \
void DynamicArray_##name##_append(DynamicArray_##name* array, T value) { \
    while (array->size >= array->capacity) { \
        DynamicArray_##name##_grow(array); \
    } \
 \
    array->data[array->size] = value; \
    array->size++; \
} \
 \
void DynamicArray_##name##_grow(DynamicArray_##name* array) { \
    if (array->capacity > 0) { \
        array->capacity *= 2; \
    } else { \
        array->capacity = 1; \
    }\
 \
    T* new_array = realloc(array->data, array->capacity * sizeof(T)); \
 \
    if (new_array == NULL) { \
        LOG_SYS_ERROR("Realloc array failed"); \
        LOG_ERROR("Realloc size %d", array->size); \
        exit(EXIT_FAILURE); \
        return; \
    } \
 \
    array->data = new_array; \
} \
 \
void DynamicArray_##name##_destroy(DynamicArray_##name* array) { \
    free(array->data); \
    free(array); \
} \

