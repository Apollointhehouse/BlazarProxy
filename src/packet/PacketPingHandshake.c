#include "packet/PacketPingHandshake.h"

#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"
#include "util/StringUTF16.h"

static void PacketPingHandshake_handle(const void* self) {
    const PacketPingHandshake* packet = self;

    STRING_UTF16_TO_C_STR(ping_host_string, packet->ping_host_string);
    STRING_UTF16_TO_C_STR(hostname, packet->hostname);

    printf(
        "PacketPingHandshake("
        "payload=%d, "
        "identifier=%d, "
        "ping_host_string=\"%s\", "
        "protocol_version=%d, "
        "hostname=%s, "
        "port=%d)\n",
        packet->payload,
        packet->identifier,
        ping_host_string,
        packet->protocol_version,
        hostname,
        packet->port
    );
}

static void PacketPingHandshake_destroy(const void* self) {
    const PacketPingHandshake* packet = self;

    packet->ping_host_string->destroy(packet->ping_host_string);
    packet->hostname->destroy(packet->hostname);
    free((void*)packet);
}

static void* PacketPingHandshake_read(const Connection* connection) {
    int16_t temp;

    PacketPingHandshake* packet = calloc(1, sizeof(PacketPingHandshake));

    if (!packet) return NULL;

    if (
        connection_read_i8(connection, (int8_t*)&packet->payload) <= 0 ||
        connection_read_i8(connection, (int8_t*)&packet->identifier) <= 0 ||
        connection_read_str_utf16(connection, &packet->ping_host_string) <= 0 ||
        connection_read_i16(connection, &temp) <= 0 ||
        connection_read_i8(connection, (int8_t*)&packet->protocol_version) <= 0 ||
        connection_read_str_utf16(connection, &packet->hostname) <= 0 ||
        connection_read_i32(connection, &packet->port) <= 0
    ) {
        PacketPingHandshake_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketPingHandshake_write(const void* self, const Connection* connection) {
    const PacketPingHandshake* packet = self;

    STRING_UTF16_TO_C_STR(ping_host_string, packet->ping_host_string);

    connection_write_i8(connection, (int8_t)packet->payload);
    connection_write_i8(connection, (int8_t)packet->identifier);
    connection_write_str_utf16(connection, packet->ping_host_string);
    connection_write_i16(connection, 3 + (int16_t)packet->ping_host_string->size(packet->ping_host_string) + 4);
    connection_write_i8(connection, (int8_t)packet->protocol_version);
    connection_write_str_utf16(connection, packet->hostname);
    connection_write_i32(connection, packet->port);
}

PacketEntry PacketPingHandshake_vtable() {
    return (PacketEntry) {
        .read = PacketPingHandshake_read,
        .write = PacketPingHandshake_write,
        .destroy = PacketPingHandshake_destroy,
        .handle = PacketPingHandshake_handle,
    };
}
