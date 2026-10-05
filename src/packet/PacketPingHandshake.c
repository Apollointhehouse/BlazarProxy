#include "packet/PacketPingHandshake.h"

#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"
#include "ConnectionContext.h"
#include "packet/PacketDisconnect.h"
#include "packet/PacketFactory.h"
#include "util/StringUTF16.h"

#define PING_REPLY "\xA7" "1" "\0" "32769" "\0" "8.0.1" "\0" "Proxy Server" "\0" "0" "\0" "100"

static ssize_t PacketPingHandshake_handle(const Packet* packet, const ConnectionContext* ctx) {
    const PacketPingHandshake* data = packet->data;

    STRING_UTF16_TO_C_STR(ping_host_string, data->ping_host_string);
    STRING_UTF16_TO_C_STR(hostname, data->hostname);

    printf(
        "PacketPingHandshake("
        "payload=%d, "
        "identifier=%d, "
        "ping_host_string=\"%s\", "
        "protocol_version=%d, "
        "hostname=%s, "
        "port=%d)\n",
        data->payload,
        data->identifier,
        ping_host_string,
        data->protocol_version,
        hostname,
        data->port
    );

    PacketDisconnect* response_data = malloc(sizeof(PacketDisconnect));
    if (!response_data) return 1;

    response_data->reason = string_utf16_from_c_str(PING_REPLY, sizeof(PING_REPLY) - 1);

    Packet* response = calloc(1, sizeof(Packet));
    if (!response) {
        free(response_data->reason);
        free(response_data);
        return 1;
    }

    response->data = response_data;
    response->v = PacketDisconnect_vtable();

    connection_write_i8(ctx->source, (int8_t)255);
    response->v->write(response, ctx->source);
    response->v->destroy(response);

    return 0;
}

static void PacketPingHandshake_destroy(const Packet* packet) {
    const PacketPingHandshake* data = packet->data;

    data->ping_host_string->destroy(data->ping_host_string);
    data->hostname->destroy(data->hostname);
    free((void*)data);
    free((void*)packet);
}

static const Packet* PacketPingHandshake_read(const Connection* connection) {
    int16_t temp;

    PacketPingHandshake* data = calloc(1, sizeof(PacketPingHandshake));
    if (!data) return NULL;

    Packet* packet = calloc(1, sizeof(Packet));
    if (!packet) return NULL;
    packet->data = data;
    packet->v = PacketPingHandshake_vtable();

    if (!data) return NULL;

    if (
        connection_read_i8(connection, (int8_t*)&data->payload) <= 0 ||
        connection_read_i8(connection, (int8_t*)&data->identifier) <= 0 ||
        connection_read_str_utf16(connection, &data->ping_host_string) <= 0 ||
        connection_read_i16(connection, &temp) <= 0 ||
        connection_read_i8(connection, (int8_t*)&data->protocol_version) <= 0 ||
        connection_read_str_utf16(connection, &data->hostname) <= 0 ||
        connection_read_i32(connection, &data->port) <= 0
    ) {
        PacketPingHandshake_destroy(packet);
        return NULL;
    }

    return packet;
}

static void PacketPingHandshake_write(const Packet* packet, const Connection* connection) {
    const PacketPingHandshake* data = packet->data;

    STRING_UTF16_TO_C_STR(ping_host_string, data->ping_host_string);

    connection_write_i8(connection, (int8_t)data->payload);
    connection_write_i8(connection, (int8_t)data->identifier);
    connection_write_str_utf16(connection, data->ping_host_string);
    connection_write_i16(connection, 3 + (int16_t)data->ping_host_string->size(data->ping_host_string) + 4);
    connection_write_i8(connection, (int8_t)data->protocol_version);
    connection_write_str_utf16(connection, data->hostname);
    connection_write_i32(connection, data->port);
}

const Packet_VTable* PacketPingHandshake_vtable() {
    static const Packet_VTable vtable = (Packet_VTable) {
        .read = PacketPingHandshake_read,
        .write = PacketPingHandshake_write,
        .destroy = PacketPingHandshake_destroy,
        .handle = PacketPingHandshake_handle,
    };

    return &vtable;
}

const PacketFactory* PacketPingHandshake_factory() {
    static const PacketFactory factory = {
        .read = PacketPingHandshake_read
    };

    return &factory;
}
