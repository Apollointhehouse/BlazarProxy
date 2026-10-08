cmake_minimum_required(VERSION 4.1)
project(BlazarProxy C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_CLANG_TIDY
        clang-tidy
        --removed-arg=-pedantic-errors
        -checks=-*,clang-analyzer-core.NullDereference,clang-analyzer-nullability.*
        -warnings-as-errors=*)

add_executable(BlazarProxy src/main.c
        src/proxy.c
        include/proxy.h
        src/Connection.c
        include/Connection.h
        src/packet/handshake/PacketPingHandshake.c
        include/packet/handshake/PacketPingHandshake.h
        src/util/StringUTF16.c
        include/util/StringUTF16.h
        include/packet/Packet_VTable.h
        src/ConnectionContext.c
        include/ConnectionContext.h
        src/packet/handshake/PacketDisconnect.c
        include/packet/PacketFactory.h
        src/packet/Packet.c
        include/packet/Packet.h
        src/packet/handshake/PacketHandshake.c
        include/packet/handshake/PacketHandshake.h
        include/util/StringUTF8.h
        src/util/StringUTF8.c
        include/nullability.h
        include/util/UUID.h
        include/packet/auth/PacketLogin.h
        src/packet/auth/PacketLogin.c
        src/util/UUID.c
        src/packet/auth/PacketAESSendKey.c
        include/packet/auth/PacketAESSendKey.h
        src/packet/container/PacketRecipeSync.c
        include/packet/container/PacketRecipeSync.h
        src/packet/world/PacketBlockRegionUpdate.c
        include/packet/world/PacketBlockRegionUpdate.h
        src/packet/misc/PacketCustomPayload.c
        include/packet/misc/PacketCustomPayload.h
        include/packet/misc/PacketCustomPayload.h
        src/packet/player/PacketPlayerConfig.c
        include/packet/player/PacketPlayerConfig.h
        src/util/logging.c
        include/util/logging.h
        src/packet/entity/PacketTileEntityData.c
        include/packet/entity/PacketTileEntityData.h
        include/packet/entity/PacketTileEntityData.h
        include/util/NBT.h
        src/util/NBT.c
        src/packet/handshake/PacketKeepAlive.c
        include/packet/handshake/PacketKeepAlive.h
        src/packet/world/PacketSyncIDs.c
        include/packet/world/PacketSyncIDs.h
        src/packet/entity/PacketSetSpawnPosition.c
        include/packet/entity/PacketSetSpawnPosition.h
        include/packet/world/PacketSetTime.h
        src/packet/world/PacketSetTime.c
        src/packet/player/PacketUpdatePlayerProfile.c
        include/packet/player/PacketUpdatePlayerProfile.h
        include/packet/player/PacketPlayerList.h
        src/packet/player/PacketPlayerList.c
        src/packet/entity/PacketAddMob.c
        include/packet/entity/PacketAddMob.h
        src/util/EntityDataItem.c
        include/util/EntityDataItem.h
        include/util/EntityDataItem.h
        include/util/DynamicArray.h
        include/util/Conversion.h
        src/packet/world/PacketChunkVisibility.c
        src/packet/player/PacketMovePlayerPosRot.c
        src/packet/player/PacketPlayerGamemode.c
        src/packet/container/PacketContainerSetContent.c
        src/packet/player/PacketPhotoMode.c
        include/packet/world/PacketGameRule.h
        src/packet/world/PacketGameRule.c
        src/packet/chat/PacketCommandManager.c
        include/packet/chat/PacketCommandManager.h
        src/packet/player/PacketSetHeldObject.c)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(BlazarProxy PRIVATE
            -Wno-nullability-extension
            -Xclang -pedantic-errors
            -Wno-strict-prototypes
            -Wno-deprecated-non-prototype
            -Werror=null-dereference

            -fsanitize=undefined
            -fno-sanitize-recover=undefined
    )

    target_link_options(BlazarProxy PRIVATE -fsanitize=undefined)

elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(BlazarProxy PRIVATE
            -fanalyzer
            -pedantic-errors
            -Wno-strict-prototypes
            -Werror=analyzer-null-dereference
    )
endif()

target_compile_definitions(BlazarProxy PRIVATE DEBUG)