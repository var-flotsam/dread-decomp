// The string heap's clear: frees every size class's memory, headers and free stack.
#include "base/global/UnkStringHeap.h"

using base::global::UnkStringHeap;

void fn_7100083d90(UnkStringHeap* self) {
    for (auto& c : self->classes)
        c.Free();
}
