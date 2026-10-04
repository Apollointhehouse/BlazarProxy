#include "packet/PacketPingHandshake.h"

#include <stdio.h>
#include <stdlib.h>

#include "connection.h"
#include "util/String16BE.h"

PacketEntry PacketPingHandshake_vtable() {
    return (PacketEntry) {
        .read = PacketPingHandshake_read,
        .write = PacketPingHandshake_write,
        .destroy = PacketPingHandshake_destroy,
        .handle = PacketPingHandshake_handle,
    };
}

void* PacketPingHandshake_read(const Connection* connection) {
    int16_t temp;

    PacketPingHandshake* packet = calloc(1, sizeof(PacketPingHandshake));

    if (!packet) return NULL;

    if (
        connection_read_i8(connection, (int8_t*)&packet->payload) <= 0 ||
        connection_read_i8(connection, (int8_t*)&packet->identifier) <= 0 ||
        connection_read_str_16be(connection, &packet->ping_host_string) <= 0 ||
        connection_read_i16(connection, &temp) <= 0 ||
        connection_read_i8(connection, (int8_t*)&packet->protocol_version) <= 0 ||
        connection_read_str_16be(connection, &packet->hostname) <= 0 ||
        connection_read_i32(connection, &packet->port) <= 0
    ) {
        PacketPingHandshake_destroy(packet);
        return NULL;
    }

    return packet;
}

void PacketPingHandshake_write(const void* self, const Connection* connection) {
    const PacketPingHandshake* packet = self;

    STRING_16BE_TO_C_STR(ping_host_string, packet->ping_host_string);

    connection_write_i8(connection, (int8_t)packet->payload);
    connection_write_i8(connection, (int8_t)packet->identifier);
    connection_write_str_16be(connection, packet->ping_host_string);
    connection_write_i16(connection, 3 + (int16_t)packet->ping_host_string->length + 4);
    connection_write_i8(connection, (int8_t)packet->protocol_version);
    connection_write_str_16be(connection, packet->hostname);
    connection_write_i32(connection, packet->port);
}

void PacketPingHandshake_destroy(const void* self) {
    const PacketPingHandshake* packet = self;
    string_16be_destroy(packet->ping_host_string);
    string_16be_destroy(packet->hostname);
    free((void*)packet);
}

void PacketPingHandshake_handle(const void* self) {
    const PacketPingHandshake* packet = self;

    STRING_16BE_TO_C_STR(ping_host_string, packet->ping_host_string);
    STRING_16BE_TO_C_STR(hostname, packet->hostname);

    printf(
    "handle PacketPingHandshake:\n"
        "payload: %d\n"
        "identifier: %d\n"
        "ping_host_string: %s\n"
        "protocol_version: %d\n"
        "hostname: %s\n"
        "port: %d\n",
        packet->payload,
        packet->identifier,
        ping_host_string,
        packet->protocol_version,
        hostname,
        packet->port
    );
}
