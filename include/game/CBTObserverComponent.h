#pragma once

// CBTObserverComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2216,
// 0x7100ca6844-0x7100cab490). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkPairArrays.h"
#include "types.h"

struct CBlackboard;
struct UnkGameManagerSub_0x1af8;

extern void* g_unk_7101aa52d0[];
extern char g_unk_7101aa52b0[];  // The observer's vtable.
extern void* g_unk_7101aa5328[];
void fn_7100ca7a60(UnkGameManagerSub_0x1af8* self, CBlackboard* blackboard, const base::global::CStrId* section);
void fn_7100ca9158(base::UnkPairArrays* pairs, u32 capacity);  // Reserves room for `capacity` pairs.
