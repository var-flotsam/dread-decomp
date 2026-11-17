#pragma once

// CCharClassShootActivatorComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1346,
// 0x710088d2c4-0x71008969f0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkTunable;
struct UnkProperty;
struct UnkEntry;
struct UnkEntryVector;
UnkEntry* fn_7100892528(void* self, u32 key);  // Finds or adds the entry for a key (one copy per map type).
void fn_710089566c(UnkEntryVector* v, const UnkEntry* value);
extern UnkProperty g_unk_717272c420;
void fn_7100895498(UnkEntryVector* v, u32 n);
extern UnkTunable g_unk_717272c450;
extern char g_unk_717272c9c0[];  // two listener sets
