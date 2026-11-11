#pragma once

// UnkTU1376: functions of this translation unit that other files call (tools/tu_map.py unit 1376,
// 0x71008cac70-0x71008db1d8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDoorLife;
struct UnkDoorLifeA5_b61;
struct UnkDoorLife_b44a2;
struct UnkDoorLock;
struct UnkUnit2186Object;
bool fn_71008d0ef4(void*);
void fn_71008d498c(UnkDoorLife_b44a2* self, u32 a, u32 b, u32 c);
UnkUnit2186Object* fn_71008d5d2c(void* owner);
void fn_71008cdf84(UnkDoorLock* door);
void fn_71008d0c24(UnkDoorLifeA5_b61* self, f32 target, f32 speed);
bool fn_71008d5c1c(UnkDoorLife* self);
