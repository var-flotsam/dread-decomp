#pragma once

// UnkTU54: functions of this translation unit that other files call (tools/tu_map.py unit 54,
// 0x71000769f0-0x710007b430). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAim;
struct UnkMat4f;
f32 fn_710007a1d0(f32 angle);
void fn_7100077bbc(UnkMat4f* m);
void fn_710007a170(UnkAim* aim, f32 current);
