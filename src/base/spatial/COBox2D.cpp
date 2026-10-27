// base::spatial::COBox2D's unit (tools/tu_map.py unit 107: 0x710010d604-0x7100110c54; its class
// registration is src/game_s1/fn_7100110b90.cpp): an oriented 2D box (base/spatial/COBox2D.h), its
// centre, its two unit axes (the rotation src/game_lo/fn_7100110664.cpp rebuilds), its size along
// them, then an angle offset and the angle.
// src/engine/SystemStartup.cpp has the same layout as UnkOrientedBox2D.
// Holds fn_7100110420 (a corner by index) and fn_7100110794 (whether a point lies inside). Names
// and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/spatial/COBox2D.h"
#include "types.h"

using base::math::CVector2D;
using base::spatial::COBox2D;

// Vector arithmetic on the box's points (component by component).
static inline CVector2D operator*(CVector2D v, f32 s) {
    return {v.x * s, v.y * s};
}
static inline CVector2D operator+(CVector2D a, CVector2D b) {
    return {a.x + b.x, a.y + b.y};
}
static inline CVector2D operator-(CVector2D a, CVector2D b) {
    return {a.x - b.x, a.y - b.y};
}

// Corner `index` of the box: centre -/+ half the size along axis_x (minus for 0 and 3), then -/+
// half the size along axis_y (minus for 0 and 1); the origin for any other index.
// NON_MATCHING(minor): register allocation only (the original keeps 0.5 in s0 and the point in s1/s2,
// ours in s2 and s0/s1); the same instructions in the same order.
UnkVec2Result fn_7100110420(const COBox2D* self, u32 index) {
    CVector2D p;
    switch (index) {
    case 0:
        p = self->center - self->axis_x * 0.5f * self->size.x - self->axis_y * 0.5f * self->size.y;
        break;
    case 1:
        p = self->center + self->axis_x * 0.5f * self->size.x - self->axis_y * 0.5f * self->size.y;
        break;
    case 2:
        p = self->center + self->axis_x * 0.5f * self->size.x + self->axis_y * 0.5f * self->size.y;
        break;
    case 3:
        p = self->center - self->axis_x * 0.5f * self->size.x + self->axis_y * 0.5f * self->size.y;
        break;
    default:
        return UnkVec2Result(0.0f, 0.0f);
    }
    return UnkVec2Result(p.x, p.y);
}

// The segment test of src/engine/SystemStartup.cpp (its box type is this one).
struct UnkOrientedBox2D;
struct UnkContactList2D;
bool fn_7100127bb0(const UnkOrientedBox2D* box, const UnkSegment2D* seg, UnkContactList2D* list, bool inside);

// A contact list (UnkContactList2D) with room for two contacts of 0x2c bytes.
struct UnkContactList2D_x2 {
    u32 count;
    u32 items[0x16];
};

// Whether `point` lies inside the box: the segment from the centre to it crosses no side.
bool fn_7100110794(const COBox2D* self, const CVector2D& point) {
    UnkSegment2D seg(self->center, point);
    UnkContactList2D_x2 contacts;
    contacts.count = 0;
    return !fn_7100127bb0(reinterpret_cast<const UnkOrientedBox2D*>(self), &seg,
                          reinterpret_cast<UnkContactList2D*>(&contacts), true);
}
