#pragma once

// A value that runs from `start` to `start + delta` over `duration`, shaped by a string of modifier
// letters (src/base/UnkCurveModifiers.cpp, unit 53 of tools/tu_map.py); `end` is what it reports once
// the time has run out. The letters are the data of a string at +0x18. Names are placeholders.

#include "base/global/CRntString.h"
#include "types.h"

// A value that runs from `start` to `start + delta` over `duration`, shaped by `modifiers`; `end`
// is what it reports once the time has run out.
struct UnkTimedCurve {
    f32 start;
    f32 field_0x4;
    f32 duration;
    f32 delta;
    f32 time;
    u32 field_0x14;
    const char* modifiers;
    u8 field_0x20[0x28];
    f32 end;
};

// Sets the curve up to run from `start` to `target` over `duration` (fn_7100075f78).
void fn_7100075f78(f32 start, f32 target, f32 duration, UnkTimedCurve* curve,
                   const base::global::CRntString* modifiers);
// Advances the curve's time by `dt` and returns its value there (fn_71000763c0).
f32 fn_71000763c0(f32 dt, UnkTimedCurve* curve);
