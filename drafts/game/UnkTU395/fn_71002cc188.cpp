// Unit 395: moves a 16-byte entry into a growable array of them at the given index, shifting the
// later entries up (CRntVector<T>::Insert written out). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkEntry16_l2cc188 {
    u64 field_0x0;
    u64 field_0x8;

    UnkEntry16_l2cc188(UnkEntry16_l2cc188&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};
using UnkEntries16_l2cc188 = base::global::CRntVector<UnkEntry16_l2cc188>;

// Allocates new_capacity entries and moves the entries over, freeing the old array.
static inline UnkEntry16_l2cc188* MoveTo(UnkEntries16_l2cc188* v, u32 new_capacity) {
    UnkEntry16_l2cc188* p = static_cast<UnkEntry16_l2cc188*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry16_l2cc188), 8, v->flags));
    if (UnkEntry16_l2cc188* src = v->data) {
        UnkEntry16_l2cc188* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src)
            new (dst) UnkEntry16_l2cc188(static_cast<UnkEntry16_l2cc188&&>(*src));
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71002cc188(UnkEntries16_l2cc188* self, UnkEntry16_l2cc188* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkEntry16_l2cc188* p;
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
        new (&p[index]) UnkEntry16_l2cc188(static_cast<UnkEntry16_l2cc188&&>(*value));
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = self->GrownCapacity(n);
            if (cap < grown) {
                UnkEntry16_l2cc188* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkEntry16_l2cc188* q = &self->data[i];
            new (q + 1) UnkEntry16_l2cc188(static_cast<UnkEntry16_l2cc188&&>(*q));
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkEntry16_l2cc188(static_cast<UnkEntry16_l2cc188&&>(*value));
    }
    ++self->count;
}
