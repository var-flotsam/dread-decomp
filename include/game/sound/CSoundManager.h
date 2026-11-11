#pragma once

// CSoundManager: functions of this translation unit that other files call (tools/tu_map.py unit 2646,
// 0x710101facc-0x710102cba4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDropPlayer_ovb0208;
struct UnkDropTarget;
void fn_7101024b28(UnkDropTarget* self, void* record, void* source, void* a, u32 b);
void fn_7101023f04(UnkDropTarget* target, void* entry, UnkDropPlayer_ovb0208* player, void* arg4, u32 arg5);
