// CHyperBeamGun's unit: one step of a walk over navigation-mesh sides: within its budget, takes the side's
// segment, and while the target accepts the side's far point adds the leg from the last point to it to the
// path; reaching the goal side adds the last leg to the goal and ends the walk. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

using base::math::CVector2D;

f32 fn_7100126238(const UnkSegment2D* seg);  // the segment's length

// A leg of the beam's path; its length is measured later (-1 until then), and a copy is unmeasured.
struct UnkBeamLeg_l848f74 {
    CVector2D start;
    CVector2D end;
    f32 length;

    explicit UnkBeamLeg_l848f74(const UnkSegment2D& segment) {
        start.x = segment.start.x;
        start.y = segment.start.y;
        end.x = segment.end.x;
        end.y = segment.end.y;
        length = -1.0f;
    }
    UnkBeamLeg_l848f74(const UnkBeamLeg_l848f74& other) {
        start.x = other.start.x;
        start.y = other.start.y;
        end.x = other.end.x;
        end.y = other.end.y;
        length = -1.0f;
    }
};
static_assert(sizeof(UnkBeamLeg_l848f74) == 0x14, "layout");

using UnkBeamLegs_l848f74 = base::global::CRntVector<UnkBeamLeg_l848f74>;

// Allocates new_capacity legs and copies the legs over, freeing the old array.
static inline UnkBeamLeg_l848f74* MoveTo(UnkBeamLegs_l848f74* v, u32 new_capacity) {
    UnkBeamLeg_l848f74* p = static_cast<UnkBeamLeg_l848f74*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkBeamLeg_l848f74), 8, v->flags));
    if (v->data) {
        UnkBeamLeg_l848f74* src = v->data;
        UnkBeamLeg_l848f74* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src)
            new (dst) UnkBeamLeg_l848f74(*src);
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

// Appends the leg of `segment`, growing the array when it is full.
static inline void AddLeg(UnkBeamLegs_l848f74* v, const UnkSegment2D& segment) {
    u32 n = v->count;
    u32 cap = v->capacity;
    u32 new_capacity = cap;
    if (cap <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkBeamLeg_l848f74* p;
    if (cap < new_capacity) {
        p = MoveTo(v, new_capacity);
        n = v->count;
        v->data = p;
        v->capacity = new_capacity;
    } else {
        p = v->data;
    }
    new (&p[n]) UnkBeamLeg_l848f74(segment);
    ++v->count;
}

struct UnkBeamTarget_l848f74 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual bool vf_0x50(const CVector2D& point);  // whether the beam may pass the point
};

// The side the walk ends at and the point on it.
struct UnkBeamGoal_l848f74 {
    u16 cell;
    u8 side;
    u8 pad_0x3;
    CVector2D point;
};

// The walk's state, all by reference.
struct UnkBeamWalk_l848f74 {
    const bool* reversed;
    UnkBeamLegs_l848f74* legs;
    f32* length;
    const UnkBeamGoal_l848f74* goal;
    CVector2D* last;
    UnkBeamTarget_l848f74* target;
    s32* budget;
};

// The side's point the walk heads for: its end, or its start when walking in reverse.
static inline const CVector2D& FarPoint(const UnkBeamWalk_l848f74* walk, const UnkNavMeshSideSegment& side) {
    return *walk->reversed ? side.end : side.start;
}

bool fn_7100848f74(UnkBeamWalk_l848f74* walk, const UnkNavMeshCell* cell, const u8* side) {
    if (--*walk->budget <= 0) {
        walk->legs->count = 0;
        return false;
    }
    UnkNavMeshSideSegment segment = {};
    segment.field_0x10 = -1.0f;
    if (!fn_71002afea8(cell, side, &segment))
        return false;

    if (walk->target->vf_0x50(FarPoint(walk, segment))) {
        UnkSegment2D leg(*walk->last, FarPoint(walk, segment));
        const CVector2D& point = FarPoint(walk, segment);
        walk->last->x = point.x;
        walk->last->y = point.y;
        AddLeg(walk->legs, leg);
        *walk->length += fn_7100126238(&leg);
        return true;
    }

    const UnkBeamGoal_l848f74* goal = walk->goal;
    if (*cell->Id() == goal->cell && *side == goal->side) {
        UnkSegment2D leg(*walk->last, goal->point);
        *walk->length += fn_7100126238(&leg);
        AddLeg(walk->legs, leg);
        return false;
    }
    walk->legs->count = 0;
    *walk->length = -1.0f;
    return false;
}
