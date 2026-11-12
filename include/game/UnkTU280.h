#pragma once

// UnkTU280: functions and globals of this translation unit that other files use (tools/tu_map.py unit 280,
// 0x71001fb9ac-0x71001fe698). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkSceneEntry;
struct UnkSceneManager;
struct UnkStruct_7100219588_View;
bool fn_71001fc69c(void* passes, u32 layer);
void fn_71001fc7bc(void* passes, u64 hash, const f32* value, u32 count);
UnkStruct_7100219588_View* fn_71001fce38(void* views, u32 a, u32 b);
extern UnkSceneManager* g_unk_716e598008 __attribute__((visibility("hidden")));  // An instance.
void fn_71001fe390(base::UnkVector<UnkSceneEntry>* self, const UnkSceneEntry* entry);
// Copy-assigns an entry array.
base::UnkVector<UnkSceneEntry>* fn_71001fdacc(base::UnkVector<UnkSceneEntry>* self, const base::UnkVector<UnkSceneEntry>* other);
