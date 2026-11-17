// CAbilityGhostAura unit: one step of a walk along navigation-mesh cell sides. Moves the walker toward the
// near or far end of a side (by at most the distance left), records the point and the side's normal, and
// says whether distance is left. Names and layouts are placeholders.
#include <math.h>
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

using base::math::CVector2D;

struct UnkGhostAuraWalkPath_l31526c {
    f32 walked;
    base::global::CRntVector<CVector2D> points;
    base::global::CRntVector<UnkVec2Result> normals;
};
static_assert(__builtin_offsetof(UnkGhostAuraWalkPath_l31526c, normals) == 0x30, "UnkGhostAuraWalkPath_l31526c layout");

// The walk's state, each member captured by reference.
struct UnkGhostAuraWalk_l31526c {
    UnkGhostAuraWalkPath_l31526c** path;
    CVector2D* pos;
    f32* remaining;
    const u8* direction;  // 0: toward a side's start; otherwise toward its end
    const f32* max_step;
};

// CRntVector's append, growing to the next multiple of the step when full.
template <class T>
static inline void Append(base::global::CRntVector<T>* v, const T& value) {
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

// Whether v lies within eps of zero.
static inline bool IsNearZero(f32 v, f32 eps) {
    return v - eps <= 0.0f && v + eps >= 0.0f;
}

bool fn_710031526c(const UnkGhostAuraWalk_l31526c* walk, const UnkNavMeshCell* cell, const u8* side) {
    UnkNavMeshSideSegment seg;
    seg.start.x = 0.0f;
    seg.start.y = 0.0f;
    seg.end.x = 0.0f;
    seg.end.y = 0.0f;
    seg.field_0x10 = -1.0f;
    if (!fn_71002afea8(cell, side, &seg))
        return false;

    u8 direction = *walk->direction;
    CVector2D target = direction == 0 ? seg.start : seg.end;
    CVector2D pos = *walk->pos;
    f32 dx = target.x - pos.x;
    if (direction != 0) {
        if (dx < 0.0f)
            return false;
    } else if (dx > 0.0f) {
        return false;
    }
    f32 dy = target.y - pos.y;
    f32 len = sqrtf(dx * dx + dy * dy);
    if (IsNearZero(dx, 0.1f)) {
        if (len > *walk->max_step)
            return false;
    } else {
        // Too steep a side: its direction points up too much.
        f32 inv = 1.0f / len;
        if (dy * inv + dx * inv * 0.0f >= 0.6f)
            return false;
    }

    f32 remaining = *walk->remaining;
    if (len > remaining) {
        const CVector2D* pos = walk->pos;
        f32 l = sqrtf(dx * dx + dy * dy);
        if (l != 0.0f) {
            f32 inv = 1.0f / l;
            dx *= inv;
            dy *= inv;
        }
        f32 r = *walk->remaining;
        target.x = pos->x + dx * r;
        target.y = dy * r + pos->y;
        (*walk->path)->walked += r;
        *walk->remaining = 0.0f;
    } else {
        *walk->remaining = remaining - len;
        (*walk->path)->walked += len;
    }
    *walk->pos = target;
    Append(&(*walk->path)->points, target);
    Append(&(*walk->path)->normals, fn_7100125e78(reinterpret_cast<const UnkSegment2D*>(&seg)));
    return *walk->remaining > 0.0f;
}
