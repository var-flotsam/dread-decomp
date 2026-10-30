// The type-erased array's clear (base/UnkErasedVector.h), in unit 1632 of tools/tu_map.py: destroys
// each value that holds one and empties the array, keeping its buffer.
#include "base/UnkErasedVector.h"

void fn_7100ad3588(UnkErasedVector* self) {
    for (u64 i = 0; i < self->count; ++i) {
        UnkErasedValue_0x10& v = self->data[i];
        if (v.ops) {
            v.ops->destroy(&v);
            v.ops = nullptr;
        }
    }
    self->count = 0;
}
