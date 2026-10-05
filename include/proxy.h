#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include "nullability.h"

void proxy(int32_t port);

static void accept_connection(int32_t server_fd, struct sockaddr_in address, socklen_t addrlen);

static void* nullable bridge(void* nonnull arg);