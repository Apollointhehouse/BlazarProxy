#include "packet/Packet.h"
#include "packet/handshake/PacketDisconnect.h"
#include "packet/handshake/PacketPingHandshake.h"
#include "packet/handshake/PacketHandshake.h"
#include "packet/auth/PacketLogin.h"
#include "packet/auth/PacketAESSendKey.h"
#include "packet/chat/PacketCommandManager.h"
#include "packet/container/PacketContainerSetContent.h"
#include "packet/container/PacketRecipeSync.h"
#include "packet/entity/PacketAddMob.h"
#include "packet/entity/PacketSetSpawnPosition.h"
#include "packet/entity/PacketTileEntityData.h"
#include "packet/handshake/PacketKeepAlive.h"
#include "packet/misc/PacketCustomPayload.h"
#include "packet/player/PacketAddPlayer.h"
#include "packet/player/PacketMovePlayerPosRot.h"
#include "packet/player/PacketPhotoMode.h"
#include "packet/player/PacketPlayerConfig.h"
#include "packet/player/PacketPlayerGamemode.h"
#include "packet/player/PacketPlayerList.h"
#include "packet/player/PacketSetEquippedItem.h"
#include "packet/player/PacketSetHeldObject.h"
#include "packet/player/PacketUpdatePlayerProfile.h"
#include "packet/player/PacketUpdatePlayerState.h"
#include "packet/world/PacketBlockRegionUpdate.h"
#include "packet/world/PacketChunkVisibility.h"
#include "packet/world/PacketGameRule.h"
#include "packet/world/PacketSetTime.h"
#include "packet/world/PacketSyncIDs.h"

static const PacketFactory* nullable packets[256];

static void register_packet(const uint8_t id, const PacketFactory* nonnull factory) {
    if (packets[id]) {
        printf("Packet factory already registered for id: %d\n", id);
        return;
    }

    packets[id] = factory;
}

void register_packets() {
    register_packet(0, PacketKeepAlive_factory());
    register_packet(1, PacketLogin_factory());
    register_packet(2, PacketHandshake_factory());
    register_packet(4, PacketSetTime_factory());
    register_packet(5, PacketSetEquippedItem_factory());
    register_packet(6, PacketSetSpawnPosition_factory());
    register_packet(13, PacketMovePlayerPosRot_factory());
    register_packet(19, PacketUpdatePlayerState_factory());
    register_packet(20, PacketAddPlayer_factory());
    register_packet(24, PacketAddMob_factory());
    register_packet(27, PacketSetHeldObject_factory());
    register_packet(36, PacketPlayerConfig_factory());
    register_packet(41, PacketPlayerGamemode_factory());
    register_packet(50, PacketChunkVisibility_factory());
    register_packet(51, PacketBlockRegionUpdate_factory());
    register_packet(72, PacketUpdatePlayerProfile_factory());
    register_packet(75, PacketRecipeSync_factory());
    register_packet(74, PacketGameRule_factory());
    register_packet(104, PacketContainerSetContent_factory());
    register_packet(120, PacketCommandManager_factory());
    register_packet(136, PacketAESSendKey_factory());
    register_packet(138, PacketPlayerList_factory());
    register_packet(140, PacketTileEntityData_factory());
    register_packet(143, PacketPhotoMode_factory());
    register_packet(201, PacketSyncIDs_factory());
    register_packet(250, PacketCustomPayload_factory());
    register_packet(254, PacketPingHandshake_factory());
    register_packet(255, PacketDisconnect_factory());
}

const PacketFactory* nullable get_packet_factory(const uint8_t id) {
    const PacketFactory* nullable factory = packets[id];
    if (!factory) return NULL;
    if (!factory->read) return NULL;

    return factory;
}
