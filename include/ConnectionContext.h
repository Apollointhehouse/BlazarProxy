#pragma once
#include "Connection.h"
#include "nullability.h"

typedef enum Direction {
    DIRECTION_C2S, DIRECTION_S2C
} Direction;

static char* nonnull Direction_Name[2] = { "C2S", "S2C" };

typedef struct ConnectionContext {
    const Connection* nonnull sink;
    const Connection* nonnull source;
    Direction direction;
} ConnectionContext;

ConnectionContext* nullable connection_ctx_create(const Connection* nonnull sink, const Connection* nonnull source, Direction direction);
void connection_ctx_destroy(const ConnectionContext* nonnull context);