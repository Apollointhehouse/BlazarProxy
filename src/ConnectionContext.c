#include "ConnectionContext.h"

#include <stdlib.h>

ConnectionContext* connection_ctx_create(const Connection* sink, const Connection* source) {
    ConnectionContext* context = malloc(sizeof(ConnectionContext));
    if (!context) return NULL;

    context->source = source;
    context->sink = sink;

    return context;
}
void connection_ctx_destroy(const ConnectionContext* context) {
    if (!context) return;

    free((void*)context);
}