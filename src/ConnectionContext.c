#include "ConnectionContext.h"

#include <stdlib.h>

#include "nullability.h"

ConnectionContext* nullable connection_ctx_create(const Connection* sink, const Connection* source) {
    ConnectionContext* nullable context = malloc(sizeof(ConnectionContext));
    if (!context) return NULL;

    context->source = source;
    context->sink = sink;

    return context;
}
void connection_ctx_destroy(const ConnectionContext* nonnull context) {
    if (!context) return;

    free((void*)context);
}