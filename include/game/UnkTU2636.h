#pragma once

// UnkTU2636: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2636,
// 0x7100fd10cc-0x7100fd1c7c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMusicRequest_b55;
bool fn_7100fd1ad8(UnkMusicRequest_b55* self, u32 arg);
extern void* g_unk_7101cac0c0[];

struct UnkEntry_0x150;  // game/UnkEntry_0x150.h
// Builds the entry with its kind (src/game_7100ece7fc/fn_7100fd10cc.cpp).
void fn_7100fd10cc(UnkEntry_0x150* self, s32 kind);
