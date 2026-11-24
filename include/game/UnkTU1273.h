#pragma once

// UnkTU1273: functions of this translation unit that other files call (tools/tu_map.py unit 1273,
// 0x710080f9e0-0x710080fc8c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkKey;
struct UnkSlots_926874;
UnkSlots_926874* fn_710080fa28(UnkKey* key);
bool AreKindedKeysEqual(const UnkKey* key, const UnkKey* other);
void fn_710080fb00(UnkKey* key, void* owner);
bool fn_710080fb08(UnkKey* key);
