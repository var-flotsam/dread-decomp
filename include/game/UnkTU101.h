#pragma once

// UnkTU101: functions and globals of this translation unit that other files use (tools/tu_map.py unit 101,
// 0x7100104ef4-0x71001061f4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSoundHandle;
struct UnkInterp_7100105d34;
struct UnkInterp_7100105f94;
struct UnkPlayingSound_ovb0074;
struct UnkSoundObject_b50;
struct UnkSoundVoice2_b56;
void fn_7100105d34(UnkInterp_7100105d34*, f32, bool, f32);
void fn_7100105034(void*);
void fn_7100105da8(UnkInterp_7100105d34* self, f32 value, f32 time);
void fn_7100105ef4(void* sound, f32 time, f32 step, bool arg3);
void fn_7100104f08(void* voice);
void fn_710010519c(UnkSoundVoice2_b56* voice, void* system, u32 seq, bool flag);
void fn_7100105c00(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105f94(UnkInterp_7100105f94* self, s32 level, f32 time);
void fn_7100106024(UnkSoundObject_b50*, const void*);
extern const UnkSoundHandle g_unk_7108b6c460;  // The empty handle.
void fn_7100105bac(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105c9c(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105cf0(UnkPlayingSound_ovb0074* sound, f32 value);
