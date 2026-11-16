// CCollider's unit (2503): inserts a copy of an array of 8-byte elements at an index of an array of them,
// moving the later ones up one place (growing the outer array first when it is full); appending is
// fn_7100d43e70. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

// An array of pointers that owns its storage (0x28 bytes, laid out as base::global::CRntVector).
struct UnkPtrArray_ovb0034 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkPtrArray_ovb0034(UnkPtrArray_ovb0034&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkPtrArray_ovb0034() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
static_assert(sizeof(UnkPtrArray_ovb0034) == 0x28, "UnkPtrArray_ovb0034 layout");

struct UnkPtrArrays_ovb0034 {
    UnkPtrArray_ovb0034* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100d43e70(UnkPtrArrays_ovb0034* self, const UnkPtrArray_ovb0034* value);

// Builds `dst` as a copy of `src`: its settings, the capacity reserved, then the pointers.
static inline void CopyConstruct(UnkPtrArray_ovb0034* dst, const UnkPtrArray_ovb0034* src) {
    dst->data = nullptr;
    dst->count = 0;
    dst->capacity = 0;
    dst->step = src->step;
    dst->field_0x14 = src->field_0x14;
    dst->flags = src->flags;
    dst->allocator = src->allocator;
    u32 n = src->capacity;
    if (n != 0) {
        // The array's reserve, written out: an existing array would be moved over and freed.
        void** p = static_cast<void**>(dst->allocator->vf_0x40((u64)n * 8, 8, dst->flags));
        if (dst->data) {
            __builtin_memcpy(p, dst->data, (u64)dst->count * 8);
            dst->allocator->vf_0x48(dst->data);
        }
        dst->data = p;
        dst->capacity = n;
    }
    __builtin_memcpy(dst->data, src->data, (u64)src->count * 8);
    dst->count = src->count;
}

void fn_7100d441e0(UnkPtrArrays_ovb0034* self, const UnkPtrArray_ovb0034* value, u32 index) {
    using E = UnkPtrArray_ovb0034;
    u32 n = self->count;
    if (n == index) {
        fn_7100d43e70(self, value);
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            E* p = static_cast<E*>(self->allocator->vf_0x40((u64)grown * sizeof(E), 8, self->flags));
            if (E* s = self->data) {
                E* o = p;
                for (u32 i = 0; i < self->count; ++i, ++s, ++o) {
                    new (o) E(static_cast<E&&>(*s));
                    s->~E();
                }
                self->allocator->vf_0x48(self->data);
            }
            self->data = p;
            self->capacity = grown;
        }
    }
    // The arrays from the last down to the index move up one place (the index runs through -1 when it is 0).
    for (u32 i = self->count - 1; i >= index; --i) {
        E* q = &self->data[i];
        new (q + 1) E(static_cast<E&&>(*q));
        self->data[i].~E();
        if (i == 0)
            break;
    }
    CopyConstruct(&self->data[index], value);
    ++self->count;
}
