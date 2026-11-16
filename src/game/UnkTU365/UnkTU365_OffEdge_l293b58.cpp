// UnkTU365 (tools/tu_map.py unit 365, the nav mesh generator): whether a segment leaves a polygon's border:
// false when either end is outside the polygon, true when either end or the midpoint is off the border.
// Names and layouts are placeholders.
#include <functional>
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPolygon2D;
using UnkPointFilter = std::function<bool(const CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;
void fn_7100117f98(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit);

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

bool fn_7100293b58(const UnkSegment2D* segment, UnkEntry* entry) {
    if (fn_7100116de0(entry, const_cast<CVector2D*>(&segment->start), 0, 1, 1.0f) &&
        fn_7100116de0(entry, const_cast<CVector2D*>(&segment->end), 0, 1, 1.0f)) {
        const UnkPolygon2D* polygon = reinterpret_cast<const UnkPolygon2D*>(entry);
        s32 edge;
        f32 t;
        f32 start_distance;
        f32 end_distance;
        fn_7100117f98(polygon, &segment->start, &edge, &t, nullptr, &start_distance, false, UnkPointFilter(),
                      UnkEdgeVisitor());
        fn_7100117f98(polygon, &segment->end, &edge, &t, nullptr, &end_distance, false, UnkPointFilter(),
                      UnkEdgeVisitor());
        if (!IsNearZero(start_distance) || !IsNearZero(end_distance))
            return true;

        f32 mid_distance;
        f32 mid_y = (segment->start.y + segment->end.y) * 0.5f;
        f32 mid_x = (segment->start.x + segment->end.x) * 0.5f;
        CVector2D mid = {mid_x, mid_y};
        fn_7100117f98(polygon, &mid, &edge, &t, nullptr, &mid_distance, false, UnkPointFilter(), UnkEdgeVisitor());
        if (!IsNearZero(mid_distance))
            return true;
    }
    return false;
}
