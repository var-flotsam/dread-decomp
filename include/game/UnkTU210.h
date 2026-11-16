#pragma once

// UnkTU210: globals of this translation unit that more than one file uses (tools/tu_map.py unit 210,
// 0x710016e75c-0x7100170300). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSlot_b0235;

extern UnkSlot_b0235* g_unk_716e34c420[];  // the slot table (game/UnkSlot_b0235.h)
extern u32 g_unk_716e34e428;               // the top of the slot table's used part
extern UnkSlot_b0235* g_unk_716e34e430[];  // slots queued for release
extern u32 g_unk_716e352430;               // how many are queued
