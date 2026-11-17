#pragma once

// CBreakableHintComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1166,
// 0x7100751c5c-0x71007534a8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "base/UnkDeferredSet.h"
#include "types.h"

struct UnkHintRef_ovb0093;
struct UnkBreakableHint_A3_b72;
void fn_71007523e8(UnkBreakableHint_A3_b72* self);
extern UnkPropertyEx<> g_unk_7172711390;  // BREAKABLE_HINTS.
void fn_7100752ba8(base::UnkDeferredSet<UnkHintRef_ovb0093>* set, UnkHintRef_ovb0093* item);
extern void* g_unk_7101a81968[];
