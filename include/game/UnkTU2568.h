#pragma once

// UnkTU2568: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2568,
// 0x7100ebf278-0x7100ec32c0). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNavMeshNodePool_0x70;
struct Result;
struct UnkNavMeshManager;
struct Vec2;

extern void* g_unk_7101ca9f90[];
bool fn_7100ebfd4c(UnkNavMeshManager*, Vec2*, u16, Result*, u32, f32, f32);
void fn_7100ebf2fc(UnkNavMeshNodePool_0x70* self, u32 count, u32 unused, u32 flags, base::UnkAllocator* allocator);
