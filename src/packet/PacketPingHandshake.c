#include "packet/PacketPingHandshake.h"

#include <stdlib.h>

#include "connection.h"
#include "util/String.h"

void PacketPingHandshake_write(void* self, const Connection* connection) {
    // connection_write(connection, data);
// sink.writeByte(payload.toByte())
// sink.writeByte(identifier.toByte())
// sink.writeJavaStringUTF16BE(pingHostString)
// sink.writeShort((3 + StandardCharsets.UTF_16BE.encode(pingHostString).array().size + 4).toShort())
// sink.writeByte(protocolVersion.toByte())
// sink.writeJavaStringUTF16BE(hostname)
// sink.writeInt(port)
}

void* PacketPingHandshake_create(const Connection* connection) {
    uint8_t payload;
    connection_read_i8(connection, (int8_t*)&payload);
    uint8_t identifier;
    connection_read_i8(connection, (int8_t*)&identifier);

    StringBE* ping_host_string = connection_read_str(connection);

    int16_t temp;
    connection_read_i16(connection, &temp);
    uint8_t protocol_version;
    connection_read_i8(connection, (int8_t*)&protocol_version);

    StringBE* hostname = connection_read_str(connection);

    int32_t port;
    connection_read_i32(connection, &port);

    PacketPingHandshake* packet = malloc(sizeof(PacketPingHandshake));

    packet->payload = payload;
    packet->identifier = identifier;
    packet->ping_host_string = ping_host_string;
    packet->protocol_version = protocol_version;
    packet->hostname = hostname;
    packet->port = port;

    return packet;
}

void PacketPingHandshake_destroy(void* self) {
    PacketPingHandshake* packet = self;
    string_destroy(packet->ping_host_string);
    string_destroy(packet->hostname);
    free(packet);
}

void PacketPingHandshake_handle(const void* self) {
    const PacketPingHandshake* packet = self;

    printf("handle PacketPingHandshake:\n");
    printf("payload: %d\n", packet->payload);
    printf("identifier: %d\n", packet->identifier);
    printf("ping_host_string: ");
    string_print(packet->ping_host_string);
    printf("\n");
    printf("protocol_version: %d\n", packet->protocol_version);
    printf("hostname: ");
    string_print(packet->hostname);
    printf("\n");
    printf("port: %d\n", packet->port);
}
