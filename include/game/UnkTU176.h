#pragma once

// UnkTU176: functions and globals of this translation unit that other files use (tools/tu_map.py unit 176,
// 0x7100154770-0x71001587dc). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxHeap;

// Sets the colour write mask of render target `index` (the graphics device's callback).
extern void (*g_unk_716e29b578)(void* target, s32 index, bool r, bool g, bool b, bool a);
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
extern void (*g_unk_716e29b6b0)(void* object, s32 which, s32 value);
extern void (*g_unk_716e29b9b8)(void* context, s32 which, s32 value);
extern void (*g_unk_716e29b9b0)(void* context, s32 which, s32 value);
extern void (*g_unk_716e29bbb0)(void* sync, void* device);  // NVN's sync initialiser.
extern void (*g_unk_716e29b6b8)(void* object, s32 a, s32 b, s32 c, s32 d);
extern void (*g_unk_716e29afc8)(void* pool, s32 id, void* texture, void* view);
extern void (*g_unk_716e29b0d0)(void* builder, s32 value);
extern void (*g_unk_716e29b0d8)(void* builder, s32 value);
extern void (*g_unk_716e29b100)(void* builder, s32 width, s32 height);
extern void (*g_unk_716e29b118)(void* builder, s32 format);
extern u64 (*g_unk_716e29b138)(void* builder);
extern u64 (*g_unk_716e29b140)(void* builder);
extern void (*g_unk_716e29b148)(void* builder, UnkGfxHeap* heap, u64 offset);
extern u8 (*g_unk_716e29b288)(void* texture, void* builder);
extern void (*g_unk_716e29b038)(void* builder);
