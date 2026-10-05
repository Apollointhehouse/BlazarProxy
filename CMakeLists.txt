cmake_minimum_required(VERSION 4.1)
project(BlazarProxy C)

set(CMAKE_C_STANDARD 11)

add_executable(BlazarProxy src/main.c
        src/proxy.c
        include/proxy.h
        src/Connection.c
        include/Connection.h
        src/packet/PacketPingHandshake.c
        include/packet/PacketPingHandshake.h
        src/util/StringUTF16.c
        include/util/StringUTF16.h
        src/packet/Packet_VTable.c
        include/packet/Packet_VTable.h
        src/ConnectionContext.c
        include/ConnectionContext.h
        src/packet/PacketDisconnect.c
        include/packet/PacketFactory.h
        src/packet/PacketFactory.c
        src/packet/Packet.c
        include/packet/Packet.h)

include_directories(include)
