#pragma once

// UnkTU53: functions of this translation unit that other files call (tools/tu_map.py unit 53,
// 0x7100075f78-0x71000769f0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
f32 fn_7100076020(f32 t, const char* modifiers);
f32 fn_71000768f0(f32 y);  // A clip-space coordinate moved by half a pixel of the viewport's width or height.
f32 fn_71000768c0(f32 x);  // A clip-space coordinate moved by half a pixel of the viewport's width or height.
f32 SmoothStepCubic(f32 t);
struct UnkPoint3f;  // three floats, as fn_71000764d0 reads them
// Projects a point onto a plane along a direction; the squared miss (src/game/UnkTU53/fn_71000764d0.cpp).
f32 fn_71000764d0(const UnkPoint3f* from, const UnkPoint3f* to, const UnkPoint3f* planePoint, UnkPoint3f* outHit,
                  const UnkPoint3f* givenDir, f32* outDist);
