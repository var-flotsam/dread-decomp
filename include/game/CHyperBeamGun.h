#pragma once

// CHyperBeamGun: functions of this translation unit that other files call (tools/tu_map.py unit 1305,
// 0x71008424e4-0x71008495a4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHyperBeamGun_b56;

bool fn_7100845fac(void* beam);
s32 CHyperBeamGun_GetChargeBlockFlags(void* self);
void fn_7100843a50(char* self, bool on);
void fn_71008449ec(UnkHyperBeamGun_b56* self, bool a, bool b);
void* fn_7100846678(void* self, const void* path, u32 a, u32 b);
