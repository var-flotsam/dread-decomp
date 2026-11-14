#include <new>

#include "base/UnkErasedVector.h"
#include "types.h"

// Appends a copy of `value`: the array grows first (its elements moved over), and the copy is
// made by the value's own copy function, when it has one.
// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710023b424(UnkErasedVector* self, const UnkErasedValue_0x10* value) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    u32 new_capacity = capacity;
    if (capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)  // hangs, as base::UnkVector::PushBack does
        for (;;) {
        }
    UnkErasedValue_0x10* p;
    if (capacity < new_capacity) {
        p = static_cast<UnkErasedValue_0x10*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkErasedValue_0x10), 8, self->flags));
        if (UnkErasedValue_0x10* src = self->data) {
            UnkErasedValue_0x10* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                new (dst) UnkErasedValue_0x10(static_cast<UnkErasedValue_0x10&&>(*src));
                ++dst;
                ++src;
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    p[n].ops = nullptr;
    UnkErasedOps* ops = value->ops;
    if (ops->copy) {
        // The original reloads the copy function after this store.
        __builtin_memcpy(&p[n].ops, &ops, sizeof(ops));
        ops->copy(&p[n], value);
    }
    ++self->count;
}
