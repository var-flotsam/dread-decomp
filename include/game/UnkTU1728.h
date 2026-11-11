#pragma once

// UnkTU1728: functions of this translation unit that other files call (tools/tu_map.py unit 1728,
// 0x7100b892d4-0x7100b8bb94). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAnimTiming;
struct UnkMotionTracks;
struct UnkTimeline;
void fn_7100b8b3bc(UnkTimeline* self, void* target, UnkMotionTracks* tracks, const u32* flags, f32 from, f32 to);
f32 fn_7100b8aff0(f32 seconds, const UnkAnimTiming* timing);
