// Constructs a pair of parallel arrays (growth step 10, the given flags, the default allocator)
// and reserves room for `reserve` entries.
#include "base/UnkPairArrays.h"

// NON_MATCHING(minor): the copy loop compares the widened index in 64 bits (the original compares 32 bits);
// behaviour is identical
void fn_71002d4e50(base::UnkPairArrays* self, u32 reserve, u32 flags) {
    self->keys = nullptr;
    self->values = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = 10;
    self->flags = flags;
    self->allocator = fn_7100083990();
    if (self->capacity < reserve) {
        self->Grow(reserve);
        self->capacity = reserve;
    }
}
