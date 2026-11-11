#pragma once

// CTunable: functions of this translation unit that other files call (tools/tu_map.py unit 111,
// 0x7100132c10-0x71001395b8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

// Probably whether the caller is on the sound thread (it compares a stored thread id with the current one).
bool fn_7100132cd0();

u32 fn_7100132fc0();
u32 fn_7100132fd0();
f32 fn_7100132fa0();  // The default scale.
f32 fn_7100132fe0();  // One of the screen's two aspect factors (thunks of fn_71001329e0 and fn_71001329d0).
f32 fn_7100132ff0();  // One of the screen's two aspect factors (thunks of fn_71001329e0 and fn_71001329d0).
