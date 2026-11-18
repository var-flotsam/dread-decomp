// CBreakableTileGroupSonarTargetComponent unit (tools/tu_map.py unit 1517): whether a point (on screen)
// lies within `range` of the near edge of the target tile group's box, and the group still has an intact
// tile. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/CActorComponent.h"
#include "game/CBreakableTileGroup.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEntryInfo {
    u8 pad_0x0[0x38];
    s32 field_0x38;
};

// An axis-aligned box.
struct UnkBox_la0e1d0 {
    CVector3D min;
    CVector3D max;
};

static inline CVector3D operator+(const CVector3D& a, const CVector3D& b) {
    CVector3D sum = {a.x + b.x, a.y + b.y, a.z + b.z};
    return sum;
}

static inline CVector3D operator*(const CVector3D& v, f32 scale) {
    CVector3D product = {v.x * scale, v.y * scale, v.z * scale};
    return product;
}

static inline CVector3D Center(UnkBox_la0e1d0 box) {
    return (box.min + box.max) * 0.5f;
}

// The breakable tile group: its box and its tiles.
struct UnkSonarTileGroup_la0e1d0 {
    u8 pad_0x0[0x15];
    bool enabled;  // 0x15
    u8 pad_0x16[0x2a];
    u32 count;  // 0x40
    u8 pad_0x44[0x1c];
    UnkBox_la0e1d0 box;  // 0x60
};
static_assert(__builtin_offsetof(UnkSonarTileGroup_la0e1d0, box) == 0x60, "layout");

struct UnkSonarTarget_la0e1d0 {
    u8 pad_0x0[0xa0];
    UnkSonarTileGroup_la0e1d0* group;  // 0xa0
};

struct UnkBreakableTileGroupSonarTarget_la0e1d0 {
    u8 pad_0x0[0x40];
    UnkWeakHandle* handle;  // 0x40
};

bool fn_7100a0e1d0(UnkBreakableTileGroupSonarTarget_la0e1d0* self, const CVector2D* point, f32 range) {
    if (!fn_710024fbc8(self))
        return false;
    UnkSonarTileGroup_la0e1d0* group =
        reinterpret_cast<UnkSonarTarget_la0e1d0*>(self->handle->target)->group;
    if (!group->enabled)
        return false;
    CVector2D edge;
    CVector3D center = Center(group->box);
    fn_710007b430(&edge, &center);
    f32 width = group->box.max.x - group->box.min.x;
    edge.x += point->x > edge.x ? width * 0.5f : -width * 0.5f;
    f32 height = group->box.max.y - group->box.min.y;
    edge.y += point->y > edge.y ? height * 0.5f : -height * 0.5f;
    if (fabsf(point->x - edge.x) > range || fabsf(point->y - edge.y) > range)
        return false;
    for (u32 i = 0; i < group->count; i++) {
        UnkEntryInfo* tile = fn_7100ca1ea8(group, i);
        if (tile && tile->field_0x38 == 0)
            return true;
    }
    return false;
}
