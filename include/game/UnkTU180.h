#pragma once

// UnkTU180: functions and globals of this translation unit that other files use (tools/tu_map.py unit 180,
// 0x710015c520-0x710015e4ac). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxProgram;

void fn_710015e1f0(void* state, u32 stage, u64 location, u32 count, const f32* values);
void fn_710015e020(void* state, u32 stage, u64 location, bool value);
extern u32 g_unk_716e2a1438;  // Live records of the first table.
extern u32 g_unk_716e2a3440;  // Live programs.
extern UnkGfxProgram* g_unk_716e2a3448;  // The bound program.
extern u8 g_unk_716e2a3450;  // Set when any uniform block changed.
