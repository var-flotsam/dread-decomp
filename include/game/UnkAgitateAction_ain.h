#pragma once

#include "game/UnkActor.h"
#include "game/UnkAgitateAnim_ain.h"

struct UnkAgitateAction_ain {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool active;  // 0x2c
    f32 limit;  // 0x30
    f32 time;  // 0x34
    UnkAgitateAnim_ain* anim;  // 0x38
};
