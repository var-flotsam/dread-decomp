// Unit 2702 (hint CAIComponent): an AI component's distance along x from its owner to the end of the
// path's last segment (false when the path has no segment). Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkPathOwner_l0fc554 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
};

struct UnkPathPoint_l0fc554 {
    f32 x, y, z;
};

struct UnkPathSelf_l0fc554 {
    u8 pad_0x0[0x30];
    UnkPathOwner_l0fc554* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkPathPoint_l0fc554* points;  // 0x68
    u32 count;                     // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
};

static inline const base::math::CVector2D& Pt(const UnkPathPoint_l0fc554* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

bool fn_71010fc554(base::reflection::UnkReflObject* component, f32* out) {
    UnkPathSelf_l0fc554* self = reinterpret_cast<UnkPathSelf_l0fc554*>(component);
    u32 count = self->count;
    bool open = self->closed == 0;
    u32 flag = (count != 0) & open;
    if (count == flag)
        return false;
    u32 segments = count - flag;
    f32 x = 0.0f;
    if (segments != 0) {
        u32 last = segments - 1;
        UnkPathPoint_l0fc554* points = self->points;
        UnkSegment2D seg(Pt(&points[last]), Pt(flag ? &points[segments] : &points[0]));
        x = seg.end.x;
    }
    *out = __builtin_fabsf(self->owner->field_0x44 - x);
    return true;
}
