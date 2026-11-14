#pragma once

// A pointer slot whose assignment is out of line (fn_71002a5d1c: it takes a reference on the new
// object and releases the old one). Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "types.h"

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}
