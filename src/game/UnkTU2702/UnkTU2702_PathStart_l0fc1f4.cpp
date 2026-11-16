// Unit 2702 (hint CAIComponent): the first point of an AI component's path, with the owner's
// height as third component (zero when the path has no segment). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPathOwner_l0fc1f4 {
    u8 pad_0x0[0x64];
    f32 field_0x64;
};

using base::math::CVector3D;

struct UnkPathSelf_l0fc1f4 {
    u8 pad_0x0[0x30];
    UnkPathOwner_l0fc1f4* owner;  // 0x30
    u8 pad_0x38[0x30];
    CVector3D* points;  // 0x68
    u32 count;                     // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
};

static inline const base::math::CVector2D& Pt(const CVector3D* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

UnkVec3f fn_71010fc1f4(base::reflection::UnkReflObject* component) {
    UnkPathSelf_l0fc1f4* self = reinterpret_cast<UnkPathSelf_l0fc1f4*>(component);
    u32 count = self->count;
    bool open = self->closed == 0;
    if (count == ((count != 0) & open))
        return UnkVec3f(0.0f, 0.0f, 0.0f);
    CVector3D* points = self->points;
    UnkSegment2D seg(Pt(points), Pt(count == 1 ? points : points + 1));
    UnkVec3f result;
    result.z = self->owner->field_0x64;
    result.x = seg.start.x;
    result.y = seg.start.y;
    return result;
}
