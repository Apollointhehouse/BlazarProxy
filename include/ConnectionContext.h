#pragma once
#include "Connection.h"

typedef struct ConnectionContext {
    const Connection* sink;
    const Connection* source;
} ConnectionContext;

ConnectionContext* connection_ctx_create(const Connection* sink, const Connection* source);
void connection_ctx_destroy(const ConnectionContext* context);