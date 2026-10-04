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
        src/packet/PacketEntry.c
        src/ConnectionContext.c
        include/ConnectionContext.h
        src/packet/PacketDisconnect.c)

include_directories(include)
