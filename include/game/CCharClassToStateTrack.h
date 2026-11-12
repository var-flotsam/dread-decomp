#pragma once

// CCharClassToStateTrack: functions of this translation unit that other files call (tools/tu_map.py unit 2476,
// 0x7100d29ec0-0x7100d2adb4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkReflType;
void fn_7100d2a00c(UnkReflType* self, const base::global::CStrId& name, void (*init)(), void* extra);
