#pragma once

// UnkTU20: functions of this translation unit that other files call (tools/tu_map.py unit 20,
// 0x7100012b60-0x7100013150). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAnimTrack_b67a1;
struct UnkMatrix44;
void EvaluateAnimTrackMatrixAtTime(UnkMatrix44* out, const UnkAnimTrack_b67a1* track, f32 t);
