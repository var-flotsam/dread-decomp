#pragma once

// Samus's movement (CSamusMovement's unit, 1501 of tools/tu_map.py) as the hand-written sources read it,
// and the input sample fn_71009ecf34 fills from it. Names and the layout are placeholders.

#include "game/UnkXformNode.h"
#include "types.h"

// A single float that is copied as a word.
struct UnkWordFloat {
    f32 value;
    UnkWordFloat& operator=(const UnkWordFloat& o) {
        *reinterpret_cast<a_u32*>(this) = *reinterpret_cast<const a_u32*>(&o);
        return *this;
    }
};

struct UnkSamusInputSample {
    f32 x;
    UnkWordFloat y;
};

struct UnkSamusRunInfo {
    u8 pad_0x0[0xac];
    UnkWordFloat field_0xac;  // 0xac
};

struct UnkSamusAim {
    u8 pad_0x0[0x360];
    bool field_0x360;  // 0x360
};

struct UnkSamusInputActor {
    u8 pad_0x0[0x2b0];
    UnkSamusAim* aim;  // 0x2b0
};

struct UnkSamusInputMovement {
    u8 pad_0x0[0x30];
    UnkSamusInputActor* actor;  // 0x30
    u8 pad_0x38[0x44];
    f32 field_0x7c;  // 0x7c
    u8 pad_0x80[0x764];
    bool field_0x7e4;  // 0x7e4
    u8 pad_0x7e5[0x4c73];
    UnkSamusRunInfo* info;  // 0x5458
};
static_assert(__builtin_offsetof(UnkSamusInputMovement, field_0x7e4) == 0x7e4, "UnkSamusInputMovement layout");
static_assert(__builtin_offsetof(UnkSamusInputMovement, info) == 0x5458, "UnkSamusInputMovement layout");

// Fills `out` with the movement's input sample: an x scale from the run info and its y, damped
// while slow motion or the speed booster is on, scaled while the flag at +0x7e4 is set, and x
// zeroed under some aim conditions or, with `stopWhenIdle`, while the movement is still.
void fn_71009ecf34(UnkSamusInputMovement* self, UnkSamusInputSample* out, bool stopWhenIdle);
