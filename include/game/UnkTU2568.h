#pragma once

// UnkTU2568: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2568,
// 0x7100ebf278-0x7100ec32c0). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct Result;
struct UnkNavMeshManager;
struct Vec2;

extern void* g_unk_7101ca9f90[];
bool fn_7100ebfd4c(UnkNavMeshManager*, Vec2*, u16, Result*, u32, f32, f32);
