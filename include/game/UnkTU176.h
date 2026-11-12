#pragma once

// UnkTU176: functions and globals of this translation unit that other files use (tools/tu_map.py unit 176,
// 0x7100154770-0x71001587dc). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
// An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29ac78)(void* device, s32 name, s32* value);
// NVN entry point the graphics start-up loads (inferred: the memory pool's finaliser).
extern void (*g_unk_716e29af70)(void* pool);
extern void (*g_unk_716e29b0c8)(void* builder);  // An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29b550)(void* object, s32 value);
// An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29b658)(void* object, s32 value);
// An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29b690)(void* object, u8 value);
// An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29b698)(void* object, u8 value);
// An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29b6a0)(void* object, s32 value);
// An NVN C entry point, stored by the loader (its name is unknown).
extern void (*g_unk_716e29b6a8)(void* object, u8 value);
extern void (*g_unk_716e29bbb0)(void* sync, void* device);  // NVN's sync initialiser.
