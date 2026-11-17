// Unit 1454: while the rope is taut, works out the swing speed from the grapple tunable and the owner's
// movement, clamps the change against the current speed and sets the resulting point as the movement's
// target. Names and layouts are placeholders.
#include <math.h>
#include "game/CMovementComponent.h"
#include "game/CSamusGrappleMovementState.h"
#include "game/UnkTU1450.h"
#include "types.h"

struct UnkMove_b152;
f32 fn_71009e5114(UnkMove_b152* self);

struct UnkMovementVec_71010d08d4 {
    f32 x, y, z;
};

// The grapple movement tunable (g_unk_717273ba88).
struct UnkGrappleTunable_l98558c {
    u8 pad_0x0[0x54];
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
};

static inline const UnkGrappleTunable_l98558c& GrappleTunable() {
    return reinterpret_cast<const UnkGrappleTunable_l98558c&>(g_unk_717273ba88);
}

struct UnkGrappleOwner_l98558c {
    u8 pad_0x0[0x38];
    UnkMove_b152* movement;  // 0x38
    u8 pad_0x40[0xb38];
    f32 field_0xb78;
    f32 field_0xb7c;
};
static_assert(__builtin_offsetof(UnkGrappleOwner_l98558c, field_0xb78) == 0xb78, "layout");

struct UnkGrappleLink_l98558c {
    u8 pad_0x0[0x8];
    UnkGrappleOwner_l98558c* owner;
};

struct UnkGrapple_l98558c {
    u8 pad_0x0[0x8];
    UnkGrappleLink_l98558c* link;  // 0x8
    u8 pad_0x10[0x3b4];
    f32 field_0x3c4;
    f32 field_0x3c8;
    f32 field_0x3cc;

    UnkGrappleOwner_l98558c* Owner() const { return link->owner; }
};
static_assert(__builtin_offsetof(UnkGrapple_l98558c, field_0x3c4) == 0x3c4, "layout");

struct UnkVec3_l98558c {
    f32 x, y, z;
};

struct UnkRope_l98558c {
    u8 pad_0x0[0x4];
    f32 field_0x4;
    u8 pad_0x8[0x4];
    f32 tension;  // 0xc
    u8 pad_0x10[0xc];
    UnkVec3_l98558c dir;  // 0x1c
    UnkVec3_l98558c pos;  // 0x28
    u8 pad_0x34[0x4];
    f32 speed;  // 0x38
};
static_assert(__builtin_offsetof(UnkRope_l98558c, speed) == 0x38, "layout");

void* fn_710097f1c8(UnkGrapple_l98558c* self);

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    return a + (b - a) * t;
}

void fn_710098558c(UnkGrapple_l98558c* self, const UnkRope_l98558c* rope, const f32* dt) {
    if (!(rope->tension > 1e-5f))
        return;

    f32 pull = self->Owner()->field_0xb78 * 2.0f;
    f32 t = !(pull >= 1.0f) ? pull : 1.0f;
    f32 boosted = Lerp(0.0f, 0.1f, t) + 1.0f;
    f32 scale = !(pull <= 0.0f) ? boosted : 1.0f;
    f32 dir = self->field_0x3c4;

    f32 swing = 0.0f;
    if (fn_710097f1c8(self) && rope->tension > 1e-5f) {
        UnkGrappleOwner_l98558c* owner = self->Owner();
        UnkMove_b152* movement = owner->movement;
        f32 reach = owner->field_0xb7c * (1.0f - GrappleTunable().field_0x5c);
        f32 a = fn_71009e5114(movement) * reach;
        f32 drop = owner->field_0xb7c - rope->field_0x4;
        drop = !(drop < 0.0f) ? drop : 0.0f;
        f32 energy = a - fn_71009e5114(movement) * drop;
        if (energy > 0.0f)
            swing = sqrtf(energy * 2.0f);
    }

    f32 delta = dir * (scale * swing) - rope->speed;
    if (!(self->field_0x3c4 * delta > 0.0f))
        return;

    const UnkGrappleTunable_l98558c& tunable = GrappleTunable();
    f32 r = self->field_0x3cc / tunable.field_0x58;
    r = !(r >= 1.0f) ? r : 1.0f;
    f32 limit = *dt * (tunable.field_0x54 * (1.0f - r * r));
    f32 low = delta > -limit ? delta : -limit;
    f32 high = delta < limit ? delta : limit;
    f32 step = self->field_0x3c4 <= 0.0f ? low : high;

    UnkMovementVec_71010d08d4 target;
    target.x = step * rope->dir.x + rope->pos.x;
    target.y = step * rope->dir.y + rope->pos.y;
    target.z = step * rope->dir.z + rope->pos.z;
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self))),
                  &target, true);
}
