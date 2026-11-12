#pragma once

// UnkTU1134: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1134,
// 0x710070d188-0x710070e70c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCutsceneTimeline;
struct UnkTrack;
struct UnkCls_710117af10;
struct UnkObj_710117af10;
struct UnkTrackObject_71011781f4;
void fn_710070d5dc(u64 value);
UnkCls_710117af10** fn_710070d7f8(UnkObj_710117af10* obj);
void fn_710070d874(UnkTrackObject_71011781f4* obj, bool flag, bool flag2);
bool fn_710070d830(UnkObj_710117af10* obj);
extern void* g_unk_7101a6c048[];
UnkCutsceneTimeline* fn_710070d4e8(UnkTrack* track);
u32 fn_710070d840(UnkTrack* track);
f32 fn_710070d850(UnkTrack* track);
f32 fn_710070d890(UnkTrack* track);
f32 fn_710070d89c(UnkTrack* track);
