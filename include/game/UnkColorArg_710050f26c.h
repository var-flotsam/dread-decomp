#pragma once

#include "game/UnkRGB_710050f26c.h"
#include "game/UnkTU140.h"
#include "types.h"

struct UnkColorArg_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
    UnkColorArg_710050f26c(const UnkRGB_710050f26c& rgb_, f32 a_) : rgb(rgb_), a(a_) {}
    UnkColorArg_710050f26c(const UnkColorArg_710050f26c& o) : rgb(o.rgb), a(o.a) {}
    // From four consecutive floats, word by word.
    explicit UnkColorArg_710050f26c(const f32* c) {
        ((a_u32*)&rgb)[0] = ((const a_u32*)c)[0];
        ((a_u32*)&rgb)[1] = ((const a_u32*)c)[1];
        ((a_u32*)&rgb)[2] = ((const a_u32*)c)[2];
        ((a_u32*)&a)[0] = ((const a_u32*)c)[3];
    }
};
