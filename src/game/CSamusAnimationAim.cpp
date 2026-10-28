// From unit 1140 of tools/tu_map.py (0x71007132b0-0x71007142e8, CSamusAnimationComponent by
// reflection): the aim angle the animation's "Angle" parameter is fed. Names and layouts are
// placeholders.
// Holds fn_7100713e2c.
#include "../game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Wraps an angle into (-pi, pi] (as src/game_lo/fn_7100110664.cpp declares it).
f32 fn_710007a1d0(f32 angle);

// The aim tunables (reached through the GOT; src/game_s2/fn_7100359310.cpp declares the global so):
// the limits read at +0xa8..+0xc0.
struct UnkAimTunable;
extern UnkAimTunable g_unk_71726b1438;
static inline f32 AimLimit(u64 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71726b1438) + offset);
}

// The aim component the angle comes from: its current mode object and two angles.
struct UnkAimMode {
    u8 pad_0x0[0x10];
    u32 kind;
};
struct UnkAimSource {
    u8 pad_0x0[0x50];
    UnkAimMode* mode;
    u8 pad_0x58[0x260];
    f32 field_0x2b8;
    u8 pad_0x2bc[0x84];
    f32 field_0x340;
};
struct UnkSamusAnimOwner {
    u8 pad_0x0[0x2b0];
    UnkAimSource* aim;
};

struct UnkSamusAnimation {
    u8 pad_0x0[0x30];
    UnkSamusAnimOwner* owner;
    u8 pad_0x38[0x48];
    unk_prop_bag::UnkPropBag params;  // 0x80
    u8 pad_after_params[0x530 - 0x80 - sizeof(unk_prop_bag::UnkPropBag)];
    s32 mode;     // 0x530
    u8 pad_0x534[0x4];
    f32 blend;    // 0x538, fades the angle out in mode 3
};
static_assert(__builtin_offsetof(UnkSamusAnimation, mode) == 0x530, "UnkSamusAnimation placeholder layout");
static_assert(__builtin_offsetof(UnkAimSource, field_0x2b8) == 0x2b8, "UnkAimSource placeholder layout");
static_assert(__builtin_offsetof(UnkAimSource, field_0x340) == 0x340, "UnkAimSource placeholder layout");

// Sets the animation's "Angle" (degrees) from the aim: the aim angle, clamped to the vertical or
// faded out over time depending on the mode, or the second angle in the aim's modes 2 and 3.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100713e2c(UnkSamusAnimation* self, f32 dt) {
    static CStrId s_angle("Angle", true);
    UnkAimSource* aim = self->owner->aim;
    if (!aim)
        return;
    f32 angle;
    if (!aim->mode || (aim->mode->kind & ~1u) != 2) {
        angle = fn_710007a1d0(aim->field_0x340);
        f32 blend;
        if (self->mode == 3) {
            if (!(angle < AimLimit(0xbc)) || angle <= AimLimit(0xc0)) {
                blend = self->blend + dt / -0.2f;
                blend = blend <= 0.0f ? 0.0f : blend;
            } else {
                blend = dt + dt + self->blend;
                blend = 1.0f < blend ? 1.0f : blend;
            }
            self->blend = blend;
        } else {
            self->blend = 0.0f;
            blend = 0.0f;
            if (angle > AimLimit(0xa8))
                angle = 1.5707964f;
            else if (angle < AimLimit(0xb8))
                angle = -1.5707964f;
        }
        angle = angle * (1.0f - blend);
    } else {
        angle = fn_710007a1d0(aim->field_0x2b8);
        self->blend = 0.0f;
    }
    unk_prop_bag::fn_710033187c(&self->params, s_angle, (angle * 180.0f) / 3.1415927f);
}
