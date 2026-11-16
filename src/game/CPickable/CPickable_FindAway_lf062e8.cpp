// CPickable unit (2591): the position of the first of the pickable's objects (+0x20, count +0x30) that is
// not within 0.001 of the target (+0x7c) on every axis, or the target when all are. Names and layouts are
// placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkOffsetObj_1159_l7452d0;
UnkVec3f fn_71007452d0(const UnkOffsetObj_1159_l7452d0* self);

struct UnkPickable_lf062e8 {
    u8 pad_0x0[0x20];
    UnkOffsetObj_1159_l7452d0* objects[2];  // 0x20
    u32 count;                              // 0x30
    u8 pad_0x34[0x7c - 0x34];
    UnkVec3f target;  // 0x7c
};
static_assert(__builtin_offsetof(UnkPickable_lf062e8, target) == 0x7c, "layout");

static inline bool IsNear(f32 value, f32 target) {
    const f32 eps = 0.001f;
    return value - eps <= target && value + eps >= target;
}

UnkVec3f fn_7100f062e8(UnkPickable_lf062e8* self) {
    for (u32 i = 0; i < self->count; ++i) {
        const UnkVec3f& pos = fn_71007452d0(self->objects[i]);
        if (!IsNear(pos.x, self->target.x) || !IsNear(pos.y, self->target.y) || !IsNear(pos.z, self->target.z))
            return pos;
    }
    return self->target;
}
