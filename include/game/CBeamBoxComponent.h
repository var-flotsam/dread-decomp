#pragma once

// CBeamBoxComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1355,
// 0x71008a47e0-0x71008acaa8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry;
struct UnkEntryVector;
void fn_71008ab4e0(UnkEntryVector* v, u32 n);
UnkEntry* fn_71008a84e4(void* self, u32 key);  // Finds or adds the entry for a key (one copy per map type).
void fn_71008ab6b4(UnkEntryVector* v, const UnkEntry* value);
