#include <new>

#include "base/UnkErasedVector.h"

// A value being copied: its function table is set only when the copy hook exists.
struct UnkTmpValue_b0217 {
    void* storage;
    UnkErasedOps* ops;
    UnkTmpValue_b0217() : ops(nullptr) {}
    ~UnkTmpValue_b0217() {
        if (ops) {
            ops->destroy(reinterpret_cast<UnkErasedValue_0x10*>(this));
            ops = nullptr;
        }
    }
};

void fn_7100ad3404(UnkErasedVector* self, const UnkErasedValue_0x10* values, u64 count) {
    u32 n = count;
    self->step = 10;
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x14 = true;
    self->flags = 0xf;
    self->allocator = fn_7100083990();
    if (self->capacity < n) {
        auto* p = static_cast<UnkErasedValue_0x10*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkErasedValue_0x10), 8, self->flags));
        if (self->data) {
            UnkErasedValue_0x10* src = self->data;
            UnkErasedValue_0x10* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkErasedValue_0x10(static_cast<UnkErasedValue_0x10&&>(*src));
                src->~UnkErasedValue_0x10();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkErasedValue_0x10* it = values, *end = it + count; it != end; ++it) {
        UnkTmpValue_b0217 tmp;
        UnkErasedOps* ops = it->ops;
        if (ops->copy) {
            tmp.ops = ops;
            ops->copy(reinterpret_cast<UnkErasedValue_0x10*>(&tmp), it);
        }
        fn_710023b424(self, reinterpret_cast<const UnkErasedValue_0x10*>(&tmp));
    }
}
