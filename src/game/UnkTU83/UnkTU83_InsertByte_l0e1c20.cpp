// Unit 83: inserts a byte into an array of bytes at the given index (CRntVector<T>::Insert
// written out), shifting the later ones up. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

using UnkBytes_l0e1c20 = base::global::CRntVector<u8>;

// Allocates new_capacity bytes and copies the bytes over, freeing the old array.
static inline u8* MoveTo(UnkBytes_l0e1c20* v, u32 new_capacity) {
    u8* p = static_cast<u8*>(v->allocator->vf_0x40(new_capacity, 8, v->flags));
    if (v->data) {
        __builtin_memcpy(p, v->data, v->count);
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71000e1c20(UnkBytes_l0e1c20* self, u8* value, u32 index) {
    u32 n = self->count;
    u32 cap = self->capacity;
    u8* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
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
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                u8* q = MoveTo(self, grown);
                self->data = q;
                self->capacity = grown;
                n = self->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            u8* q = &self->data[i];
            q[1] = *q;
            if (i == 0)
                break;
        }
        p = self->data;
    }
    p[index] = *value;
    ++self->count;
}
