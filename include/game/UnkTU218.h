#pragma once

// UnkTU218: functions of this translation unit that other files call (tools/tu_map.py unit 218,
// 0x710019706c-0x7100199fcc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
void fn_7100199420(void* self, const u64* hash, bool value);
void fn_7100198aac(void* self);
// Looks a slot of the holder's slot table up by id (game/UnkSlots.h).
bool fn_7100199d88(void* table, const u64* key, u32* id);
