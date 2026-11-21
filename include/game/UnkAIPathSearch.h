#pragma once

// The AI path follower's side search (fn_710035e8d8) and what it fills. Names and the layout are
// placeholders.

#include <functional>

#include "base/math/CVector.h"
#include "types.h"

struct UnkAIPathFollower;

// What a side search fills (0x14 bytes).
struct UnkAIPathSearchOut {
    u64 a;
    u64 b;
    f32 distance;

    UnkAIPathSearchOut() : a(0), b(0), distance(-1.0f) {}
};

// The side search from `from` toward `to`: `blocked` tells whether it stopped on a side, `point`
// where.
bool fn_710035e8d8(UnkAIPathFollower* self, const base::math::CVector2D* from, const base::math::CVector2D* to,
                   const base::math::CVector2D* dir_a, const base::math::CVector2D* dir_b, bool flag,
                   base::math::CVector2D* point, u32* out_0x7, f32 limit, bool* blocked, f32* out_a, f32* out_b,
                   u32* out_c, UnkAIPathSearchOut* out, std::function<void()> filter);
