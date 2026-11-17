// CAbilityGhostAura unit: one step of a walk along navmesh cell sides, as a callback over a side: moves the
// walk's position to the side's far point (or as far towards it as the distance left allows) and records
// the point and the side's normal. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector2D;

// A 2D point copied word by word.
struct UnkPoint_l315658 {
    f32 x, y;

    UnkPoint_l315658(const UnkPoint_l315658& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
    }
    explicit UnkPoint_l315658(const CVector2D& v) {
        ((a_u32*)this)[0] = ((const a_u32*)&v)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&v)[1];
    }
    explicit UnkPoint_l315658(const UnkVec2Result& v) {
        ((a_u32*)this)[0] = ((const a_u32*)&v)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&v)[1];
    }
    UnkPoint_l315658& operator=(const UnkPoint_l315658& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        return *this;
    }
};

// The walk so far: its length, the points reached and the normals of the sides crossed.
struct UnkSideWalk_l315658 {
    f32 length;
    u8 pad_0x4[0x4];
    CRntVector<UnkPoint_l315658> points;  // 0x8
    CRntVector<UnkPoint_l315658> normals;  // 0x30
};

// The callback's captures, all by reference.
struct UnkSideWalkStep_l315658 {
    UnkSideWalk_l315658** walk;
    f32* remaining;
    UnkPoint_l315658* position;
    bool* reversed;
    f32* max_distance;
};

// CRntVector's append, growing to the next multiple of the step when full.
template <class T>
static void Append(CRntVector<T>* v, const T& value) {
    u32 n = v->count;
    u32 cap = v->capacity;
    u32 new_capacity = cap;
    if (cap <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    T* p;
    if (cap < new_capacity) {
        p = static_cast<T*>(v->allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, v->flags));
        if (v->data) {
            T* src = v->data;
            T* dst = p;
            for (u32 i = 0; i < v->count; ++i, ++dst, ++src)
                new (dst) T(*src);
            v->allocator->vf_0x48(v->data);
        }
        n = v->count;
        v->data = p;
        v->capacity = new_capacity;
    } else {
        p = v->data;
    }
    new (&p[n]) T(value);
    ++v->count;
}

// Scales `v` to unit length, leaving a zero vector as it is.
static inline void NormalizeByReciprocal(CVector2D& v) {
    f32 length = Length(v);
    if (length != 0.0f) {
        f32 inv = 1.0f / length;
        v.x *= inv;
        v.y *= inv;
    }
}

bool fn_7100315658(UnkSideWalkStep_l315658* self, const UnkNavMeshCell* cell, const u8* side) {
    UnkNavMeshSideSegment segment;
    segment.start.x = 0.0f;
    segment.start.y = 0.0f;
    segment.end.x = 0.0f;
    segment.end.y = 0.0f;
    segment.field_0x10 = -1.0f;
    if (!fn_71002afea8(cell, side, &segment))
        return false;
    bool reversed = *self->reversed;
    UnkPoint_l315658 point(!reversed ? segment.end : segment.start);
    CVector2D d;
    d.x = point.x - self->position->x;
    d.y = point.y - self->position->y;
    if (reversed ? d.x < 0.0f : d.x > 0.0f)
        return false;
    if (d.y > 0.001f)
        return false;
    f32 distance = Length(d);
    if (d.x + 0.1f >= 0.0f && d.x - 0.1f <= 0.0f && distance > *self->max_distance)
        return false;
    if (distance > *self->remaining) {
        UnkPoint_l315658* position = self->position;
        NormalizeByReciprocal(d);
        f32 remaining = *self->remaining;
        point.x = position->x + d.x * remaining;
        point.y = position->y + d.y * remaining;
        (*self->walk)->length += remaining;
        *self->remaining = 0.0f;
    } else {
        *self->remaining -= distance;
        (*self->walk)->length += distance;
    }
    *self->position = point;
    Append(&(*self->walk)->points, point);
    UnkSideWalk_l315658* walk = *self->walk;
    UnkVec2Result normal = fn_7100125e78(reinterpret_cast<const UnkSegment2D*>(&segment));
    Append(&walk->normals, UnkPoint_l315658(normal));
    return true;
}
