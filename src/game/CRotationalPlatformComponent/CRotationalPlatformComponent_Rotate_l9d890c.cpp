// CRotationalPlatformComponent's unit (tools/tu_map.py unit 1498): the rotation's step. While rotating, the
// elapsed time eases the owner's z angle from 0 to the target angle over 0.3 s (a quarter sine); once the
// angle reaches the target the owner's timeline gets an id set, once. Past 2 s the rotation stops. Names and
// layouts are placeholders.
#include <math.h>

#include "../../game_s2/UnkSpitSend.h"
#include "game/CAutoolAIComponent.h"
#include "game/CComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern const u64 g_unk_710164b450;

struct UnkRotationalPlatformOwner_l9d890c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x32c];
    u8 dirty[4];  // 0x3c0: byte flags; [0] is set when the transform changes (a byte array: the original reloads after the store)
};
static_assert(__builtin_offsetof(UnkRotationalPlatformOwner_l9d890c, dirty) == 0x3c0, "layout");

struct UnkRotationalPlatform_l9d890c {
    u8 pad_0x0[0x30];
    UnkRotationalPlatformOwner_l9d890c* owner;  // 0x30
    u8 pad_0x38[0x50];
    bool rotating;  // 0x88
    bool reached;   // 0x89
    u8 pad_0x8a[0x2];
    f32 time;   // 0x8c
    f32 angle;  // 0x90, in degrees
};

static const f32 kHalfPi = 1.5707964f;
static const f32 kDegToRad = 0.017453292f;

// From `from` to `to` along a quarter sine as `t` goes from 0 to 1.
static inline f32 EaseSine(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - sinf(t * kHalfPi)) * from + to * sinf(t * kHalfPi);
}

void fn_71009d890c(UnkRotationalPlatform_l9d890c* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (!self->rotating)
        return;
    if (self->time > 2.0f) {
        self->rotating = false;
        return;
    }
    self->time += dt;
    f32 angle = EaseSine(0.0f, self->angle, self->time / 0.3f) * kDegToRad;

    UnkRotationalPlatformOwner_l9d890c* owner = self->owner;
    UnkXform& xf = owner->xform;
    if ((xf.flags & 0xc) == 8) {
        xf.scale = fn_7100076cec(&xf);
        xf.flags |= 4;
    }
    xf.flags = (xf.flags & ~8u) | 2;
    xf.angle.x = 0.0f;
    xf.angle.y = 0.0f;
    xf.angle.z = angle;
    owner->dirty[0] = 1;

    f32 current = self->owner->xform.GetAngle().z;
    if (self->angle * kDegToRad >= current && !self->reached) {
        if (auto* timeline = reinterpret_cast<UnkTimelineComponent*>(
                fn_71003c6c3c(reinterpret_cast<UnkEvacPart_b61*>(self->owner)))) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_710164b450, &mask.id, false, -1.0f);
        }
        self->reached = true;
    }
}
