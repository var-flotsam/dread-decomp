#pragma once

// The search every copy of the engine's state machine makes for a state by id: the states sit in a fixed array
// of pointers after a u32 count, and the original walks them with a byte count it tests after the shift,
// stepping the pointer before the count. Matched by fn_71008d0ef4, fn_7100a09cd4 and the other users of this
// header. Names are placeholders.
#include "types.h"

template <class StatePtr, class Id>
static inline StatePtr FindStateById(StatePtr* p, u32 count, Id id) {
    u64 size = (u64)count * sizeof(StatePtr);
    // The original tests the byte count, not the count, for zero (the barrier keeps the shift).
    asm("" : "+r"(size));
    for (; size != 0; ++p, size -= sizeof(StatePtr)) {
        if ((*p)->id == id)
            return *p;
    }
    return nullptr;
}
