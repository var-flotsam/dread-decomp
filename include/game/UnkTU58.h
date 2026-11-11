#pragma once

// UnkTU58: functions of this translation unit that other files call (tools/tu_map.py unit 58,
// 0x710007fdb4-0x71000815a4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxFrameSource;
struct UnkTimer_7100080c40;
void* fn_71000810b0(u32 value);
s64 fn_7100080b7c(UnkGfxFrameSource* timer);  // The timer's time in microseconds.
s64 fn_7100080fb4(const UnkTimer_7100080c40* self);
