// Unit 108: inserts a 3D point into an array of points at the given index (CRntVector<T>::Insert
// written out), shifting the later ones up. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "types.h"

using UnkPoints_l122b2c = base::global::CRntVector<UnkVec3f>;

// Allocates new_capacity points and copies the points over, freeing the old array.
static inline UnkVec3f* MoveTo(UnkPoints_l122b2c* v, u32 new_capacity) {
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

void fn_7100122b2c(UnkInsertTarget* self, const UnkArgObj* point, u32 index) {
    UnkPoints_l122b2c* v = reinterpret_cast<UnkPoints_l122b2c*>(self);
    const UnkVec3f& value = *reinterpret_cast<const UnkVec3f*>(point);
    u32 cap = v->capacity;
    u32 n = v->count;
    if (n == index) {
        UnkVec3f* p;
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = v->GrownCapacity(index);
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(v, new_capacity);
            index = v->count;
            v->data = p;
            v->capacity = new_capacity;
        } else {
            p = v->data;
        }
        new (&p[index]) UnkVec3f(value);
    } else {
        if (cap <= n && v->step != 0) {
            u32 grown = v->GrownCapacity(n);
            if (cap < grown) {
                UnkVec3f* q = MoveTo(v, grown);
                v->data = q;
                v->capacity = grown;
                n = v->count;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkVec3f* q = &v->data[i];
            new (q + 1) UnkVec3f(*q);
            if (i == 0)
                break;
        }
        new (&v->data[index]) UnkVec3f(value);
    }
    ++v->count;
}
