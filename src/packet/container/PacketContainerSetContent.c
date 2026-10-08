#include "packet/container/PacketContainerSetContent.h"

#include <inttypes.h>

#include "packet/Packet.h"

#include <stdio.h>
#include <stdlib.h>

#include "packet/PacketFactory.h"
#include "util/logging.h"

static ssize_t read_stack(const Connection* connection, ItemStack** stack);
static void write_stack(const Connection* connection, ItemStack* stack);

static ssize_t PacketContainerSetContent_handle(const Packet* nonnull packet, const ConnectionContext* nonnull ctx) {
    const PacketContainerSetContent* data = packet->data;

    LOG_DEBUG("PacketContainerSetContent(window_id=%d, state_id=%d, list_size=%d)", data->window_id, data->state_id, data->list_size);

    return 1;
}

static void PacketContainerSetContent_destroy(const Packet* nonnull packet) {
    const PacketContainerSetContent* data = packet->data;

    if (data->carried_item) ItemStack_destroy(data->carried_item);
    if (data->stack_list) {
        for (int i = 0; i < data->list_size; i++) {
            if (data->stack_list[i]) ItemStack_destroy(data->stack_list[i]);
        }

        free(data->stack_list);
    }

    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketContainerSetContent_read(const Connection* nonnull connection) {
    PacketContainerSetContent* data = calloc(1, sizeof(PacketContainerSetContent));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketContainerSetContent_vtable();

    if (
        connection_read_i8(connection, &data->window_id) <= 0 ||
        connection_read_i32(connection, &data->state_id) < 0
    ) {
        LOG_ERROR("Failed to read Window_ID or STATE_ID");
        PacketContainerSetContent_destroy(packet);
        return NULL;
    }

    if (read_stack(connection, &data->carried_item) <= 0) {
        LOG_ERROR("Failed to read Carried Item");
        PacketContainerSetContent_destroy(packet);
        return NULL;
    }

    if (connection_read_i16(connection, &data->list_size) <= 0) {
        LOG_ERROR("Failed to read List Size");
        PacketContainerSetContent_destroy(packet);
        return NULL;
    }

    data->stack_list = calloc(data->list_size, sizeof(ItemStack*));

    for (int i = 0; i < data->list_size; i++) {
        read_stack(connection, &data->stack_list[i]);
    }

    return packet;
}

static void PacketContainerSetContent_write(const Packet* nonnull packet, const Connection* nonnull connection) {
    const PacketContainerSetContent* data = packet->data;

    connection_write_i8(connection, data->window_id);
    connection_write_i32(connection, data->state_id);
    write_stack(connection, data->carried_item);
    connection_write_i16(connection, data->list_size);

    for (int i = 0; i < data->list_size; i++) {
        write_stack(connection, data->stack_list[i]);
    }
}

const Packet_VTable* nonnull PacketContainerSetContent_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketContainerSetContent_read,
        .write = PacketContainerSetContent_write,
        .destroy = PacketContainerSetContent_destroy,
        .handle = PacketContainerSetContent_handle,
    };

    return &vtable;
}

const PacketFactory* nonnull PacketContainerSetContent_factory() {
    static const PacketFactory factory = {
        .read = PacketContainerSetContent_read
    };

    return &factory;
}

static ssize_t read_stack(const Connection* connection, ItemStack** stack) {
    int16_t item_id;

    if (connection_read_i16(connection, &item_id) <= 0) return -1;

    if (item_id < 0) {
        *stack = NULL;
        return 1;
    }

    int8_t size;
    int16_t meta;
    NBT* tag = malloc(sizeof(NBT));

    connection_read_i8(connection, &size);
    connection_read_i16(connection, &meta);
    connection_read_nbt(connection, tag);

    *stack = malloc(sizeof(ItemStack));
    if (!*stack) return -1;

    (*stack)->item_id = item_id;
    (*stack)->item_count = size;
    (*stack)->item_meta = meta;
    (*stack)->item_nbt = tag;

    return 1;
}

static void write_stack(const Connection* connection, ItemStack* stack) {
    if (!stack) {
        connection_write_i16(connection, -1);
    } else {
        connection_write_i16(connection, stack->item_id);
        connection_write_i8(connection, stack->item_count);
        connection_write_i16(connection, stack->item_meta);
        connection_write_nbt(connection, stack->item_nbt);
    }
}