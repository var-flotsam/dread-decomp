#pragma once

// CRumbleComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1500,
// 0x71009d9dc0-0x71009dc3bc). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkHandle.h"
#include "types.h"

struct CRumbleComponent;
struct UnkRumbleParams;
base::UnkHandle fn_71009dacbc(CRumbleComponent* self, const UnkRumbleParams* params, bool a, bool b);
