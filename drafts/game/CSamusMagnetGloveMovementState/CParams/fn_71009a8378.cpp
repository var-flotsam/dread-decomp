// In the CSamusMagnetGloveMovementState::CParams unit: the magnet surface's segment, pulled in at both ends along
// a direction. The neighbouring segments of the path are looked at first (their slope angle is computed and
// unused). Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkMagnetState_l9a8378;

struct UnkPathPoint_l9a8378 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPath_l9a8378 {
    UnkPathPoint_l9a8378* points;
    u32 count;
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c

    bool IsLast(u32 i) const { return !closed && i >= NumSegments() - 1; }
    u32 NumSegments() const { return closed || count == 0 ? count : count - 1; }
    UnkSegment2D Segment(u32 i) const {
        return UnkSegment2D(points[i].pos, i == count - 1 ? points[0].pos : points[i + 1].pos);
    }
};
static_assert(__builtin_offsetof(UnkPath_l9a8378, closed) == 0x2c, "layout");

static inline bool IsNear(f32 value, f32 target, f32 tolerance) {
    return value + tolerance >= target && value - tolerance <= target;
}

static inline f32 Dot(const UnkVec2Result& a, f32 x, f32 y) {
    return a.x * x + a.y * y;
}

// The slope of a neighbouring segment; the angle itself is not used.
static inline void LookAtNeighbour(const UnkSegment2D& seg) {
    UnkVec2Result normal = fn_7100125e78(&seg);
    if (!IsNear(fabsf(Dot(normal, 1.0f, 0.0f)), 1.0f, 0.1f)) {
        f32 up = Dot(normal, 0.0f, -1.0f);
        if (!IsNear(up, 1.0f, 0.1f)) {
            f32 angle = acosf(up);
            (void)angle;
        }
    }
}

UnkSegment2D fn_71009a8378(UnkMagnetState_l9a8378* self, const UnkSegment2D* seg, const UnkPath_l9a8378* path,
                           u32 index, const CVector2D* dir) {
    UnkSegment2D result;  // field_0x14 unset in the original
    result.start.x = seg->start.x;
    result.start.y = seg->start.y;
    result.end.x = seg->end.x;
    result.end.y = seg->end.y;
    result.field_0x10 = -1.0f;
    if (!path->IsLast(index)) {
        u32 next = index == path->NumSegments() - 1 ? 0 : index + 1;
        LookAtNeighbour(path->Segment(next));
    }
    if (index != 0 || path->closed) {
        u32 prev = (index != 0 ? index : path->NumSegments()) - 1;
        LookAtNeighbour(path->Segment(prev));
    }
    f32 scale = 42.0f / fabsf(seg->start.x - seg->end.x);
    f32 len = fn_7100125fbc(seg);
    CVector2D step = {len * dir->x, len * dir->y};
    CVector2D offset = {step.x * scale, step.y * scale};
    result.start = {seg->start.x + offset.x, seg->start.y + offset.y};
    result.end = {seg->end.x - offset.x, seg->end.y - offset.y};
    result.field_0x10 = -1.0f;
    return result;
}
