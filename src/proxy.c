#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <string.h>
#include "proxy.h"
#include <errno.h>
#include "ConnectionContext.h"
#include "Connection.h"
#include "packet/PacketFactory.h"
#include "packet/Packet_VTable.h"
#include "util/logging.h"

void proxy(const int32_t port) {
    int32_t server_fd;
    struct sockaddr_in address;
    const int32_t opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_SYS_ERROR("Creating server socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR, &opt,sizeof(opt))) {
        LOG_SYS_ERROR("Setting socket options failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address)) < 0) {
        LOG_SYS_ERROR("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        LOG_SYS_ERROR("Socket listen failed");
        exit(EXIT_FAILURE);
    }

    register_packets();

    LOG_INFO("Listening for connections on port %d", port);

    while (1) {
        accept_connection(server_fd, address, sizeof(address));
    }

    close(server_fd);
}

static void accept_connection(const int32_t server_fd, struct sockaddr_in address, socklen_t addrlen) {
    const int32_t new_socket = accept(server_fd, (struct sockaddr *) &address, &addrlen);

    if (new_socket < 0) {
        LOG_SYS_ERROR("Failed to accept connection");
        return;
    }

    Connection *client_con = connection_create(new_socket);
    if (!client_con) {
        LOG_SYS_ERROR("Failed to create client connection");
        return;
    }

    pthread_t thread;

    if (pthread_create(&thread, NULL, bridge, client_con)) {
        LOG_SYS_ERROR("Failed to create connection thread");
        connection_destroy(client_con);
        return;
    }
    pthread_detach(thread);
}

static int32_t connect_to_server(const char* nonnull host, const char* nonnull port) {
    struct addrinfo hints, *res, *rp;
    int32_t fd = -1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) != 0) {
        LOG_SYS_ERROR("getaddrinfo failed for %s:%s", host, port);
        return -1;
    }

    for (rp = res; rp != NULL; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
        close(fd);
        fd = -1;
    }

    freeaddrinfo(res);
    if (fd < 0) LOG_SYS_ERROR("connect to server failed");
    return fd;
}

static void* packet_forwarding(void* nonnull args) {
    const ConnectionContext* ctx = (ConnectionContext*)args;

    const Connection* source = ctx->source;
    const Connection* sink = ctx->sink;
    const char* direction_name = Direction_Name[ctx->direction];

    uint8_t packet_id;

    while (connection_read_i8(source, (int8_t*)&packet_id) > 0) {
        LOG_INFO("packet_id: %d, direction: %s", packet_id, direction_name);

        const PacketFactory* factory = get_packet_factory(packet_id);
        if (!factory) {
            LOG_ERROR("Missing packet factory for id: %d", packet_id);
            break;
        }

        const Packet* packet = factory->read(source);

        if (!packet) {
            LOG_ERROR("Failed to read packet id: %d", packet_id);
            break;
        }

        if (!packet->v->handle(packet, ctx)) {
            packet->v->destroy(packet);
            continue;
        }
        connection_write_i8(sink, (int8_t)packet_id);
        packet->v->write(packet, sink);
        packet->v->destroy(packet);
    }

    connection_shutdown(source);
    connection_shutdown(sink);

    connection_ctx_destroy(ctx);

    return NULL;
}

static void* bridge(void* nonnull arg) {
    const Connection* client_con = arg;
    Connection* nullable server_con = NULL;
    ConnectionContext* nullable s2c_conetxt = NULL;
    ConnectionContext* nullable c2s_context = NULL;
    pthread_t c2s_thread, s2c_thread;
    int c2s_running = 0;
    int s2c_running = 0;

    LOG_INFO("Accepted Connection!");

    const int32_t server_fd = connect_to_server("btanarchy.com", "25565");
    if (server_fd < 0) {
        LOG_ERROR("connect to btanarchy.com failed");
        goto cleanup;
    }

    server_con = connection_create(server_fd);
    if (!server_con) {
        LOG_ERROR("connection_create failed");
        close(server_fd);
        goto cleanup;
    }

    s2c_conetxt = connection_ctx_create(client_con, server_con, DIRECTION_S2C);
    c2s_context = connection_ctx_create(server_con, client_con, DIRECTION_C2S);
    if (!s2c_conetxt || !c2s_context) {
        LOG_ERROR("Failed to create connection contexts");
        goto cleanup;
    }

    if (pthread_create(&c2s_thread, NULL, packet_forwarding, s2c_conetxt)) {
        LOG_ERROR("Failed to create S2C thread");
        goto cleanup;
    }
    c2s_running = 1;
    s2c_conetxt = NULL;

    if (pthread_create(&s2c_thread, NULL, packet_forwarding, c2s_context)) {
        LOG_ERROR("Failed to create C2S thread");
        connection_shutdown(client_con);
        connection_shutdown(server_con);
        goto cleanup;
    }
    s2c_running = 1;
    c2s_context = NULL;

    cleanup:
    if (s2c_running) pthread_join(s2c_thread, NULL);
    if (c2s_running) pthread_join(c2s_thread, NULL);
    if (s2c_conetxt) connection_ctx_destroy(s2c_conetxt);
    if (c2s_context) connection_ctx_destroy(c2s_context);
    if (server_con) connection_destroy(server_con);
    connection_destroy(client_con);

    LOG_INFO("Connection Closed");
    return NULL;
}