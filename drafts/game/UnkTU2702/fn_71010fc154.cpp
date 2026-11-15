// Unit 2702 (hint CAIComponent): the end of an AI component's path's last segment, with the owner's
// height as third component (zero when the path has no segment). Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPathOwner_l0fc154 {
    u8 pad_0x0[0x64];
    f32 field_0x64;
};

struct UnkPathPoint_l0fc154 {
    f32 x, y, z;
};

struct UnkPathSelf_l0fc154 {
    u8 pad_0x0[0x30];
    UnkPathOwner_l0fc154* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkPathPoint_l0fc154* points;  // 0x68
    u32 count;                     // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
};

static inline const base::math::CVector2D& Pt(const UnkPathPoint_l0fc154* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

UnkVec3f fn_71010fc154(base::reflection::UnkReflObject* component) {
    UnkPathSelf_l0fc154* self = reinterpret_cast<UnkPathSelf_l0fc154*>(component);
    u32 count = self->count;
    u8 closed = self->closed;
    u32 segments = count - ((count != 0) & (closed == 0));
    if (segments == 0)
        return UnkVec3f(0.0f, 0.0f, 0.0f);
    UnkPathPoint_l0fc154* points = self->points;
    UnkSegment2D seg(Pt(points + (segments - 1)), Pt(closed == 0 && count != 0 ? points + segments : points));
    UnkVec3f result;
    result.z = self->owner->field_0x64;
    result.x = seg.end.x;
    result.y = seg.end.y;
    return result;
}
