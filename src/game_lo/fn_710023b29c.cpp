// Copy-assigns an engine array of 16-byte type-erased values: destroys the current elements,
// grows the buffer to the other array's count if needed, then appends a copy of each element.
#include "base/UnkVector.h"

struct UnkErasedValue_0x10;

// Function table of a type-erased value; only the slots used here are named.
struct UnkErasedOps {
    void* field_0x0[6];
    void (*copy)(UnkErasedValue_0x10* dst, const UnkErasedValue_0x10* src);
    void (*move)(UnkErasedValue_0x10* dst, UnkErasedValue_0x10* src);
    void (*destroy)(UnkErasedValue_0x10* self);
};

struct UnkErasedValue_0x10 {
    void* storage;
    union {
        UnkErasedOps* ops;
    };

    UnkErasedValue_0x10(UnkErasedValue_0x10&& other) {
        ops = other.ops;
        if (ops && ops->move)
            ops->move(this, &other);
        other.ops = nullptr;
    }
    ~UnkErasedValue_0x10() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

struct UnkErasedVector {
    UnkErasedValue_0x10* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Appends a copy of `value`.
void fn_710023b424(UnkErasedVector* self, const UnkErasedValue_0x10* value);

UnkErasedVector* fn_710023b29c(UnkErasedVector* self, const UnkErasedVector* other) {
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            self->data[i].~UnkErasedValue_0x10();
        self->count = 0;

        u32 n = other->capacity;
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

        for (const UnkErasedValue_0x10* it = other->data, *end = it + other->count; it != end; ++it)
            fn_710023b424(self, it);
    }
    return self;
}
