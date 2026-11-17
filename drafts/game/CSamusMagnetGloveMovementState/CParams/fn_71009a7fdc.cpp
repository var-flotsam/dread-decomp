// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009a7fdc returns a magnet path segment pulled in
// along a direction at both ends, by less where the neighbouring segment is a wall or a 45-degree slope.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPathPoint_l9a7fdc {
    CVector2D pos;
    f32 z;
};

struct UnkPath_l9a7fdc {
    UnkPathPoint_l9a7fdc* points;
    u32 count;
    u8 pad_0xc[0x20];
    bool closed;  // 0x2c
};

inline u32 SegmentCount(const UnkPath_l9a7fdc* path) {
    return path->closed ? path->count : path->count - (path->count != 0);
}

inline UnkSegment2D GetSegment(const UnkPath_l9a7fdc* path, u32 i) {
    u32 j = i == path->count - 1 ? 0 : i + 1;
    return UnkSegment2D(path->points[i].pos, path->points[j].pos);
}

inline bool IsNear(f32 value, f32 target, f32 tolerance) {
    return value + tolerance >= target && value - tolerance <= target;
}

inline bool InRange(f32 value, f32 low, f32 high) {
    return value >= low && value <= high;
}

inline f32 Dot(const UnkVec2Result& a, f32 x, f32 y) {
    return a.x * x + a.y * y;
}

// Whether the segment with normal `normal` is a wall or a 45-degree slope.
inline bool IsSteep(const UnkVec2Result& normal) {
    if (IsNear(fabsf(Dot(normal, 1.0f, 0.0f)), 1.0f, 0.1f))
        return true;
    f32 cos = Dot(normal, 0.0f, -1.0f);
    f32 angle = IsNear(cos, 1.0f, 0.1f) ? 0.0f : acosf(cos);
    return IsNear(angle, 0.785398185f, 0.1f);
}

inline void SetPoints(UnkSegment2D* seg, const CVector2D& start, const CVector2D& end) {
    seg->start.x = start.x;
    seg->start.y = start.y;
    seg->end.x = end.x;
    seg->end.y = end.y;
    seg->field_0x10 = -1.0f;
}

UnkSegment2D fn_71009a7fdc(void* self, const UnkSegment2D* seg, const UnkPath_l9a7fdc* path, u32 index,
                           const CVector2D* dir) {
    UnkSegment2D result;  // field_0x14 is unset in the original
    SetPoints(&result, seg->start, seg->end);
    f32 length = fn_7100125fbc(seg);
    f32 inset = InRange(100.0f, length - 0.001f, length + 0.001f) ? 50.0f : 42.0f;
    f32 start_inset = inset;
    f32 end_inset = inset;
    if (path->closed || index < SegmentCount(path) - 1) {
        u32 next = index == SegmentCount(path) - 1 ? 0 : index + 1;
        UnkSegment2D side = GetSegment(path, next);
        end_inset = IsSteep(fn_7100125e78(&side)) ? 42.0f : inset;
    }
    if (index != 0 || path->closed) {
        u32 prev = (index == 0 ? SegmentCount(path) : index) - 1;
        UnkSegment2D side = GetSegment(path, prev);
        start_inset = IsSteep(fn_7100125e78(&side)) ? 42.0f : inset;
    }
    CVector2D start = {start_inset * dir->x + seg->start.x, start_inset * dir->y + seg->start.y};
    CVector2D end = {seg->end.x - end_inset * dir->x, seg->end.y - end_inset * dir->y};
    SetPoints(&result, start, end);
    return result;
}
