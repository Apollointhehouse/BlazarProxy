#include "packet/PacketPingHandshake.h"

#include <stdio.h>
#include <stdlib.h>

#include "connection.h"
#include "util/String16BE.h"

void* PacketPingHandshake_read(const Connection* connection) {
    int16_t temp;

    PacketPingHandshake* packet = malloc(sizeof(PacketPingHandshake));

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
    // connection_write(connection, data);
    // sink.writeByte(payload.toByte())
    // sink.writeByte(identifier.toByte())
    // sink.writeJavaStringUTF16BE(pingHostString)
    // sink.writeShort((3 + StandardCharsets.UTF_16BE.encode(pingHostString).array().size + 4).toShort())
    // sink.writeByte(protocolVersion.toByte())
    // sink.writeJavaStringUTF16BE(hostname)
    // sink.writeInt(port)
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
    // printf("payload: %d\n", packet->payload);
    // printf("identifier: %d\n", packet->identifier);
    // printf("ping_host_string: ");
    // string_print(packet->ping_host_string);
    // printf("\n");
    // printf("protocol_version: %d\n", packet->protocol_version);
    // printf("hostname: ");
    // string_print(packet->hostname);
    // printf("\n");
    // printf("port: %d\n", packet->port);
}
