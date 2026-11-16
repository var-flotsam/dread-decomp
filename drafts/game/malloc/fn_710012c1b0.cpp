// malloc unit (tools/tu_map.py unit 110): where two 2D segments meet. Crossing segments meet at a point;
// parallel ones overlap along the part of each that lies on the other. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCachedSegment2D;
f32 fn_7100126238(UnkCachedSegment2D* self);

// The segment's normal (unscaled).
UnkVec2Result fn_7100126298(const UnkSegment2D* seg);

struct UnkContact_710012be70;
bool fn_710012be70(const UnkSegment2D* a, const UnkSegment2D* b, UnkContact_710012be70* out, bool collinear,
                   f32 tolerance);

struct UnkMeetContact {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    u32 field_0x24;
    u32 flags;
};

static inline f32 LengthSq(const UnkSegment2D* seg) {
    return fn_7100126238(reinterpret_cast<UnkCachedSegment2D*>(const_cast<UnkSegment2D*>(seg)));
}

// Setting either end clears the cached length.
static inline void SetStart(UnkSegment2D* seg, const CVector2D& p) {
    seg->start.x = p.x;
    seg->start.y = p.y;
    seg->field_0x10 = -1.0f;
}

static inline void SetEnd(UnkSegment2D* seg, const CVector2D& p) {
    seg->end.x = p.x;
    seg->end.y = p.y;
    seg->field_0x10 = -1.0f;
}

bool fn_710012c1b0(const UnkSegment2D* a, const UnkSegment2D* b, UnkSegment2D* out) {
    f32 dx = a->end.x - a->start.x;
    f32 dy = a->end.y - a->start.y;
    UnkVec2Result normal = fn_7100126298(b);
    f32 dot = dx * normal.x + dy * normal.y;
    if (dot + 0.001f >= 0.0f && dot - 0.001f <= 0.0f) {
        // Parallel: the longer segment holds the overlap's ends.
        const UnkSegment2D* longer;
        const UnkSegment2D* shorter;
        if (LengthSq(a) > LengthSq(b)) {
            longer = a;
            shorter = b;
        } else {
            longer = b;
            shorter = a;
        }
        if (fn_7100125c64(longer, &shorter->start, false, 0.0f)) {
            SetStart(out, shorter->start);
            if (fn_7100125c64(longer, &shorter->end, false, 0.0f)) {
                SetEnd(out, shorter->end);
                return true;
            }
        } else {
            if (!fn_7100125c64(longer, &shorter->end, false, 0.0f))
                return false;
            SetStart(out, shorter->end);
        }
        SetEnd(out, fn_7100125c64(shorter, &longer->start, false, 0.0f) ? longer->start : longer->end);
        return true;
    }

    UnkMeetContact contact = {};
    if (!fn_710012be70(a, b, reinterpret_cast<UnkContact_710012be70*>(&contact), false, 0.0f))
        return false;
    // Both ends at the crossing point (dropped to 2D).
    CVector2D point;
    fn_710007b430(&point, &contact.point);
    SetStart(out, point);
    fn_710007b430(&point, &contact.point);
    SetEnd(out, point);
    return true;
}
