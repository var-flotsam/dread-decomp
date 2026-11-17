#pragma once

// time: functions and globals of this translation unit that other files use (tools/tu_map.py unit 45,
// 0x710006b6a4-0x710006d2a0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkTimelineEvent;
struct UnkTimelineEventList;
struct UnkSoundHolder;
void fn_710006cd9c(UnkSoundHolder* holder);  // Constructor.
void fn_710006b6a4(void* self);
void fn_710006cea8(UnkSoundHolder* holder, bool value);
// The event of `cls` playing at `frame`, or null.
UnkTimelineEvent* fn_710006bb80(f32 frame, UnkTimelineEventList* events, const base::reflection::CClass* cls, s32 frame_count);
// Converts a narrow string into at most `capacity` UTF-32 characters.
void fn_710006c000(const base::global::CRntString* src, wchar_t* dst, u32 capacity);
extern void* g_unk_7101a1a160[];
