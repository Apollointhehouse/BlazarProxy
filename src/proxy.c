#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "proxy.h"


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

    Connection *connection = connection_create(new_socket);
    pthread_t thread;

    if (pthread_create(&thread, NULL, bridge, connection)) {
        connection_destroy(connection);
        perror("Failed to create connection thread");
        return;
    }
    pthread_detach(thread);
}

static void* bridge(void* arg) {
    printf("bridge thread started ------------------------:\n");
    const Connection* connection = arg;

    uint8_t packet_id;

    while (connection_read_i8(connection, (int8_t*)&packet_id) > 0) {
        const PacketEntry* entry = get_packet_entry(packet_id);

        if (!entry) break;

        printf("--------------------------\n");

        printf("packet_id: %d\n", packet_id);

        const void* packet = entry->read(connection);

        if (!packet) break;

        entry->handle((void*)packet);
        entry->destroy((void*)packet);
    }

    printf("bridge thread ended\n");
    connection_destroy(arg);
    return NULL;
}