#pragma once
#include "Connection.h"
#include "nullability.h"

typedef struct ConnectionContext {
    const Connection* nonnull sink;
    const Connection* nonnull source;
} ConnectionContext;

ConnectionContext* nullable connection_ctx_create(const Connection* nonnull sink, const Connection* nonnull source);
void connection_ctx_destroy(const ConnectionContext* nonnull context);