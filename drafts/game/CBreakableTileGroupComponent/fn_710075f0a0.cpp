// CBreakableTileGroupComponent's unit (tools/tu_map.py): CRntVector<T>::Insert for a 16-byte element,
// moving it in at the given index and shifting the elements after it up.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkItem16_l75f0a0 {
    u64 field_0x0;
    u64 field_0x8;
};

using UnkItem16Vector_l75f0a0 = base::global::CRntVector<UnkItem16_l75f0a0>;

static UnkItem16_l75f0a0* Grow(UnkItem16Vector_l75f0a0* self, u32 new_capacity) {
    UnkItem16_l75f0a0* p = static_cast<UnkItem16_l75f0a0*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkItem16_l75f0a0), 8, self->flags));
    if (self->data) {
        __builtin_memcpy(p, self->data, (u64)self->count * sizeof(UnkItem16_l75f0a0));
        self->allocator->vf_0x48(self->data);
    }
    return p;
}

void fn_710075f0a0(UnkItem16Vector_l75f0a0* self, UnkItem16_l75f0a0* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkItem16_l75f0a0* p;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = Grow(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        new (&p[index]) UnkItem16_l75f0a0(*value);
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkItem16_l75f0a0* p = Grow(self, grown);
                n = self->count;
                self->data = p;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkItem16_l75f0a0* q = &self->data[i];
            new (q + 1) UnkItem16_l75f0a0(*q);
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkItem16_l75f0a0(*value);
    }
    ++self->count;
}
