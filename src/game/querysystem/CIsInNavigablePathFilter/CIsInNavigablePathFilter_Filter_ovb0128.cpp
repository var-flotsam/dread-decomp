// querysystem::CIsInNavigablePathFilter's unit (tools/tu_map.py, unit 385): whether a candidate passes the
// filter: always when the actor has no AI component (+0x240) or its path (+0x68) is not in use; otherwise
// when the candidate's position, projected to 2D, is more than 0.1 away from the path (fn_7100117ba0, the
// squared distance to the nearest edge). Names and layouts are placeholders.
#include <functional>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The nearest edge of a polygon to a point (src/base/spatial/UnkPolygon2D.cpp, which defines the types).
struct UnkPolygon2D;
using UnkPointFilter = std::function<bool(const CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;
void fn_7100117ba0(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit);

// The polygon's layout as src/base/spatial/UnkPolygon2D.cpp has it.
struct UnkNavPath_ovb0128 {
    void* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;  // 0x2c

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
};

struct UnkNavAI_ovb0128 {
    u8 pad_0x0[0x68];
    UnkNavPath_ovb0128 path;  // 0x68
};

struct UnkNavActor_ovb0128 {
    u8 pad_0x0[0x240];
    UnkNavAI_ovb0128* ai;  // 0x240
};

struct UnkNavCandidate_ovb0128 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

bool fn_71002c1e84(void* self, UnkNavActor_ovb0128* actor, UnkNavCandidate_ovb0128* candidate) {
    UnkNavAI_ovb0128* ai = actor->ai;
    if (!ai)
        return true;
    const UnkNavPath_ovb0128* path = &ai->path;
    if (path->EdgeCount() == 0)
        return true;
    s32 edge;
    f32 t;
    f32 distance;
    CVector2D point;
    f32 x = candidate->x;
    f32 y = candidate->y;
    f32 z = candidate->z;
    CVector3D pos = {x, y, z};
    fn_710007b430(&point, &pos);
    fn_7100117ba0(reinterpret_cast<const UnkPolygon2D*>(path), &point, &edge, &t, nullptr, &distance, false,
                  UnkPointFilter(), UnkEdgeVisitor());
    return !(distance - 0.1f <= 0.0f) | !(distance + 0.1f >= 0.0f);
}
