cmake_minimum_required(VERSION 4.1)
project(BlazarProxy C)

set(CMAKE_C_STANDARD 11)

add_executable(BlazarProxy src/main.c
        src/proxy.c
        include/proxy.h
        src/connection.c
        include/connection.h
        src/packet/PacketPingHandshake.c
        include/packet/PacketPingHandshake.h
        src/util/String16BE.c
        include/util/String16BE.h
        src/packet/PacketEntry.c
        src/ConnectionContext.c
        include/ConnectionContext.h)

include_directories(include)
