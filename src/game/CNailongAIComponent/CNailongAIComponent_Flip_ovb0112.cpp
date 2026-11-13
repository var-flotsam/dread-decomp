// CNailongAIComponent's unit (tools/tu_map.py): when the facing (+0x148dc) points against the owner's
// transform (+0x38, fn_710002614c; its x axis, outside a 0.01 band), the facing is flipped, and on a
// change of side (+0x1483c) the owner's controller (+0x220) gets ids 4 and 8 the other way round (as
// CNailongAIComponent_Facing_A3_b75.cpp). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"
#include "base/UnkMatrix44.h"
#include "game/UnkShakernautId.h"

struct UnkShakernautCtrl_b53;

struct UnkNailongFlipOwner_ovb0112 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x1a8];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkNailongFlip_ovb0112 {
    u8 pad_0x0[0x30];
    UnkNailongFlipOwner_ovb0112* owner;  // 0x30
    u8 pad_0x38[0x14804];
    u32 side;  // 0x1483c
    u8 pad_0x14840[0x9c];
    f32 direction;  // 0x148dc
};
static_assert(__builtin_offsetof(UnkNailongFlip_ovb0112, direction) == 0x148dc, "layout");

void fn_7100621630(UnkNailongFlip_ovb0112* self) {
    const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
    f32 x = m.m[2], y = m.m[6];
    f32 dir = self->direction;
    f32 z = m.m[10];
    f32 d = y * 0.0f + x * dir;
    d = z * 0.0f + d;
    if (!(d < 0.0f))
        return;
    if (d + 0.01f >= 0.0f && d - 0.01f <= 0.0f)
        return;
    u32 side = dir < 0.0f;
    self->direction = -dir;
    if (self->side == side)
        return;
    UnkShakernautCtrl_b53* ctrl = self->owner->ctrl;
    self->side = side;
    UnkShakernautId_b53 a1;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
    if (dir < 0.0f) {
        a1.value = 4;
        fn_71011785a8(ctrl, &a1, true, false);
        b1.value = 8;
        fn_7101178564(ctrl, &b1, true, false);
    } else {
        a2.value = 8;
        fn_71011785a8(ctrl, &a2, true, false);
        b2.value = 4;
        fn_7101178564(ctrl, &b2, true, false);
    }
}
