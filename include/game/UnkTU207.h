#pragma once

// UnkTU207: functions of this translation unit that other files call (tools/tu_map.py unit 207,
// 0x710016a6bc-0x710016b51c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkEntry_u207;

void fn_710016ae30(void* self);
void fn_710016b21c(base::global::CRntVector<UnkEntry_u207>* self, u32 capacity);
