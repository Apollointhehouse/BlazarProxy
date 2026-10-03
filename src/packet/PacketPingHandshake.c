#include "packet/PacketPingHandshake.h"

#include "connection.h"
#include "util/String.h"

void PacketPingHandshake_write(Connection* connection) {
    // connection_write(connection, data);
// sink.writeByte(payload.toByte())
// sink.writeByte(identifier.toByte())
// sink.writeJavaStringUTF16BE(pingHostString)
// sink.writeShort((3 + StandardCharsets.UTF_16BE.encode(pingHostString).array().size + 4).toShort())
// sink.writeByte(protocolVersion.toByte())
// sink.writeJavaStringUTF16BE(hostname)
// sink.writeInt(port)
}

PacketPingHandshake PacketPingHandshake_create(const Connection* connection) {
    const u_int8_t payload = connection_read_i8(connection);
    const u_int8_t identifier = connection_read_i8(connection);

    StringBE* ping_host_string = connection_read_str(connection);
    connection_read_i16(connection);
    const u_int8_t protocol_version = connection_read_i8(connection);
    StringBE* hostname = connection_read_str(connection);
    const int32_t port = connection_read_i32(connection);

    const PacketPingHandshake packet = {
        .payload = payload,
        .identifier = identifier,
        .ping_host_string = ping_host_string,
        .protocol_version = protocol_version,
        .hostname = hostname,
        .port = port
    };

    return packet;
}

void PacketPingHandshake_destroy(const PacketPingHandshake self) {
    string_destroy(self.ping_host_string);
    string_destroy(self.hostname);
}
