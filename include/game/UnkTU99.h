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
