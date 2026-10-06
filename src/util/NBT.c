#include "util/NBT.h"

#include <stdlib.h>

#include "nullability.h"

void NBT_destroy(const NBT* nonnull self) {
    free(self->buffer);
}
