// CCharClassRodomithonXAIComponent unit (tools/tu_map.py, unit 987): fn_71006681d4 copies a 16-byte entry into
// a CRntVector (base/global/CRntVector.h) at an index, shifting the later entries up one place and growing the
// array first when it is full: CRntVector::InsertCopy written out. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkEntry_l6681d4 {
    u64 field_0x0;
    u64 field_0x8;
};

using UnkEntries_l6681d4 = base::global::CRntVector<UnkEntry_l6681d4>;

// Allocates new_capacity entries and copies the entries over, freeing the old array.
static inline UnkEntry_l6681d4* MoveTo(UnkEntries_l6681d4* v, u32 new_capacity) {
    UnkEntry_l6681d4* p = static_cast<UnkEntry_l6681d4*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkEntry_l6681d4), 8, v->flags));
    if (v->data) {
        __builtin_memcpy(p, v->data, (u64)v->count * sizeof(UnkEntry_l6681d4));
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_71006681d4(UnkEntries_l6681d4* self, const UnkEntry_l6681d4* value, u32 index) {
    u32 n = self->count;
    u32 cap = self->capacity;
    UnkEntry_l6681d4* slot;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (index / self->step + 1) * self->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        UnkEntry_l6681d4* p;
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            n = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        slot = &p[n];
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = (n / self->step + 1) * self->step;
            if (cap < grown) {
                UnkEntry_l6681d4* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkEntry_l6681d4* q = &self->data[i];
            q[1] = q[0];
            if (i == 0)
                break;
        }
        slot = &self->data[index];
    }
    *slot = *value;
    ++self->count;
}