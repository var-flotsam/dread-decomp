#pragma once

#include "game/UnkTU104.h"

struct UnkSndChannelFade_l0ff4e4 {
    s32 current;  // 0x0
    s32 from;     // 0x4
    s32 to;       // 0x8
    f32 time;     // 0xc
    f32 elapsed;  // 0x10

    void Start(s32 level, f32 duration) {
        s32 target = fn_710010aaa0(level);
        from = current;
        to = target;
        time = duration;
        elapsed = 0;
    }
};
