#pragma once

// UnkTU2656: functions of this translation unit that other files call (tools/tu_map.py unit 2656,
// 0x7101053650-0x7101053e24). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkVec;
void fn_7101053850(void* effects, UnkVec* position, u32 flag, f32 duration, f32 scale);
bool fn_7101053a0c(void* effects);
bool fn_7101053a7c(void* effects);
bool fn_7101053a98(void* state);
void fn_710105383c(void* effects);
void fn_7101053c14(void* fx, const base::math::CVector3D* color, f32 a, f32 b, f32 time);
void fn_7101053968(void* effects, UnkVec3f* a, UnkVec3f* b, f32 x, f32 y, f32 z);
