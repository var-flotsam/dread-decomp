#pragma once

// UnkTU99: functions and globals of this translation unit that other files use (tools/tu_map.py unit 99,
// 0x71000f43c0-0x71001043e4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkInterp_7100105f94;
struct UnkListener_7100105f94;
struct UnkAtkHeap;
struct UnkIdMap;
struct UnkTunable;
struct UnkTunableTokens;
void fn_71000fbe1c(void* self, u32 a, f32 b, f32 c);
void fn_71000fc570(const char* text, const char* separator, UnkTunableTokens* out);
void fn_71000fc7cc(UnkTunableTokens* self);
extern UnkTunable g_unk_7101d68580;  // base::snd::CSoundSystemATK's tunable.
extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));
extern UnkIdMap* g_unk_7101d685c8;
extern UnkAtkHeap g_unk_7108b69038;
void* fn_71001039f8();
void fn_71000ffd4c(UnkListener_7100105f94* listener, UnkInterp_7100105f94* block, s32 value);
extern u8 g_unk_7108b6adb0[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6ae38[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6af50[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6afe8[] __attribute__((visibility("hidden")));
// The payload size of each command of the sound system's second queue, by command number.
extern const s32 g_unk_7101647aa8[] __attribute__((visibility("hidden")));
extern UnkAtkHeap g_unk_7108b690a8;
void fn_71000fd1d4(void* member, const u32* mask, f32 fade);
void fn_71000fea74(void* member, u32 mask, f32 fade);
