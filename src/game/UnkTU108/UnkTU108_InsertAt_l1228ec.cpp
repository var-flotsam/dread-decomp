// Unit 108: inserts a copy of a 12-byte value into a growable array of them at the given index
// (CRntVector<T>::Insert written out), shifting the later ones up. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

using UnkValues_l1228ec = base::global::CRntVector<UnkVec3f>;

// Allocates new_capacity values and copies the values over, freeing the old array.
static inline UnkVec3f* MoveTo(UnkValues_l1228ec* v, u32 new_capacity) {
    UnkVec3f* p = static_cast<UnkVec3f*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkVec3f), 8, v->flags));
    if (v->data) {
        UnkVec3f* src = v->data;
        UnkVec3f* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src)
            new (dst) UnkVec3f(*src);
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71001228ec(UnkValues_l1228ec* self, const UnkVec3f* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkVec3f* p;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = self->GrownCapacity(index);
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) UnkVec3f(*value);
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = self->GrownCapacity(n);
            if (cap < grown) {
                UnkVec3f* q = MoveTo(self, grown);
                self->data = q;
                self->capacity = grown;
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkVec3f* q = &self->data[i];
            new (q + 1) UnkVec3f(*q);
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkVec3f(*value);
    }
    ++self->count;
}
