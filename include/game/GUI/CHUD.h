#pragma once

// CHUD: functions of this translation unit that other files call (tools/tu_map.py unit 2751,
// 0x71011dce8c-0x71011e8df8). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkHudOwnedPtr;
void CHUD_EmplaceBackOwnedPtr(base::global::CRntVector<UnkHudOwnedPtr>* self, UnkHudOwnedPtr* value);
