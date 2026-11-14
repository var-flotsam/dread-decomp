#pragma once

// UnkTU2607: functions of this translation unit that other files call (tools/tu_map.py unit 2607,
// 0x7100f97d90-0x7100f9b32c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkEnvironmentFade;

// A 2D vector passed by value as a copy in memory (its copy constructor is user-provided).
struct UnkEnvVec2 {
    f32 x;
    f32 y;

    UnkEnvVec2(const UnkEnvVec2& other) {
        x = other.x;
        y = other.y;
    }
};

void fn_7100f985d8(f32 time, UnkEnvironmentFade* self, const base::math::CVector2D* to);
