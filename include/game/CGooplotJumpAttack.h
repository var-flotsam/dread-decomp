#pragma once

// CGooplotJumpAttack: functions of this translation unit that other files call (tools/tu_map.py unit 848,
// 0x7100594088-0x71005965d8). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkGooplotJump_ovb0123;
struct UnkGooplotJumpAttack;
void fn_7100594d5c(UnkGooplotJump_ovb0123* self, u32 state);
// Whether a jump towards the target is clear; the direction to it through `dir`.
bool fn_710059462c(UnkGooplotJumpAttack* self, base::math::CVector3D* dir);
