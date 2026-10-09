#pragma once
#include "packet/Packet_VTable.h"

typedef struct PacketAddPlayer {
    int32_t entity_id;
    StringUTF8* nonnull name;
    UUID uuid;
    int32_t x_position;
    int32_t y_position;
    int32_t z_position;
    int8_t rotation;
    int8_t pitch;
    int16_t current_item;
    StringUTF16* nonnull nickname;
    int8_t chat_color;
    int16_t player_config;
    StringUTF16* nonnull gamemode;
    NBT* nullable held_object_tag;
} PacketAddPlayer;

const Packet_VTable* nonnull PacketAddPlayer_vtable();
const PacketFactory* nonnull PacketAddPlayer_factory();