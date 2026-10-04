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
#include "../include/connection.h"
#include "packet/PacketEntry.h"
#include "packet/PacketPingHandshake.h"

void proxy(const int32_t port) {
    int32_t server_fd;
    struct sockaddr_in address;
    const int32_t opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR, &opt,sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address,sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    register_packets();

    while (1) {
        accept_connection(server_fd, address, sizeof(address));
    }

    close(server_fd);
}

static void accept_connection(const int32_t server_fd, struct sockaddr_in address, socklen_t addrlen) {
    const int32_t new_socket = accept(server_fd, (struct sockaddr *) &address, &addrlen);

    if (new_socket < 0) {
        perror("Failed to accept connection");
        return;
    }

    Connection *client_con = connection_create(new_socket);
    pthread_t thread;

    if (pthread_create(&thread, NULL, bridge, client_con)) {
        connection_destroy(client_con);
        perror("Failed to create connection thread");
        return;
    }
    pthread_detach(thread);
}

static int32_t connect_to_server(const char *host, const char *port) {
    struct addrinfo hints, *res, *rp;
    int32_t fd = -1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo failed for %s:%s\n", host, port);
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
    if (fd < 0) perror("connect to server failed");
    return fd;
}

static void* packet_forwarding(void* args) {
    const ConnectionContext* ctx = (ConnectionContext*)args;

    const Connection* source = ctx->source;
    const Connection* sink = ctx->sink;

    uint8_t packet_id;

    while (connection_read_i8(source, (int8_t*)&packet_id) > 0) {
        printf("packet_id: %d\n", packet_id);

        const PacketEntry* entry = get_packet_entry(packet_id);
        if (!entry) break;

        const void* packet = entry->read(source);

        if (!packet) break;

        entry->handle((void*)packet);
        connection_write_i8(sink, (int8_t)packet_id);
        entry->write((void*)packet, sink);
        entry->destroy((void*)packet);
    }

    connection_ctx_destroy(ctx);

    printf("Connection Closed\n");

    return NULL;
}

static void* bridge(void* arg) {
    printf("bridge thread started ------------------------:\n");
    const Connection* client_con = arg;

    const int32_t server_fd = connect_to_server("btanarchy.com", "25565");

    if (server_fd < 0) {
        perror("connect to btanarchy.com failed");
        exit(EXIT_FAILURE);
    }

    const Connection* server_con = connection_create(server_fd);

    if (!server_con) {
        perror("connection_create failed");
        exit(EXIT_FAILURE);
    }

    pthread_t c2s_thread;
    pthread_t s2c_thread;

    ConnectionContext* c2s_context = connection_ctx_create(client_con, server_con);
    ConnectionContext* s2c_context = connection_ctx_create(server_con, client_con);

    if (pthread_create(&c2s_thread, NULL, packet_forwarding, c2s_context)) {
        connection_ctx_destroy(c2s_context);
        perror("Failed to create C2S thread");
        return NULL;
    }
    pthread_detach(c2s_thread);

    if (pthread_create(&s2c_thread, NULL, packet_forwarding, s2c_context)) {
        connection_ctx_destroy(s2c_context);
        perror("Failed to create C2S thread");
        return NULL;
    }

    pthread_join(c2s_thread, NULL);
    pthread_join(s2c_thread, NULL);

    printf("bridge thread ended\n");
    return NULL;
}
