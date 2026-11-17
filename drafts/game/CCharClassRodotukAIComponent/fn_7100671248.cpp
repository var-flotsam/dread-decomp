// CCharClassRodotukAIComponent's unit: copies a 0x30-byte entry into a growable array of them at the
// given index, shifting the later entries up (CRntVector<T>::Insert written out). Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkEntry30_l671248 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
};
using UnkEntries30_l671248 = base::global::CRntVector<UnkEntry30_l671248>;

// Allocates new_capacity entries and copies the entries over, freeing the old array.
static inline UnkEntry30_l671248* MoveTo(UnkEntries30_l671248* v, u32 new_capacity) {
    UnkEntry30_l671248* p = static_cast<UnkEntry30_l671248*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry30_l671248), 8, v->flags));
    if (v->data) {
        __builtin_memcpy(p, v->data, (u64)v->count * sizeof(UnkEntry30_l671248));
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100671248(UnkEntries30_l671248* self, UnkEntry30_l671248* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkEntry30_l671248* p;
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
        new (&p[index]) UnkEntry30_l671248(*value);
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = self->GrownCapacity(n);
            if (cap < grown) {
                UnkEntry30_l671248* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkEntry30_l671248* q = &self->data[i];
            new (q + 1) UnkEntry30_l671248(*q);
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkEntry30_l671248(*value);
    }
    ++self->count;
}
