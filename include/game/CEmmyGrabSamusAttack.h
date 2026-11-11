#pragma once

// CEmmyGrabSamusAttack: functions and globals of this translation unit that other files use (tools/tu_map.py unit 800,
// 0x7100504d5c-0x7100511964). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkColorIn_710050f26c;
struct UnkColor_710050f26c;
struct UnkEmmyGrabAttack_A3_b67;
struct UnkGrabArg_b61;
struct UnkGrabAttack_A3_b68;
struct UnkGrabAttack_b58a3;
struct UnkGrabAttack_b59a3;
struct UnkGrabFx4_b61;
struct UnkGrabId_b59a3;
struct UnkSelf;
struct UnkStrId_71005077f8;
struct UnkStruct_71005077f8;
struct UnkStruct_710050f26c;
void fn_710050f26c(UnkStruct_710050f26c* self, f32 time, const UnkColorIn_710050f26c* color, f32 time2, const UnkColor_710050f26c* color2, f32 offset);
void fn_71005068c8(UnkEmmyGrabAttack_A3_b67* self, f32 a, f32 b, f32 c, bool d, bool e);
UnkStrId_71005077f8 fn_71005077f8(UnkStruct_71005077f8* self, u32 kind);
void fn_7100507d24(UnkGrabAttack_b58a3* self);
bool fn_710050a500(UnkGrabAttack_b59a3* self, const UnkGrabId_b59a3* id);
void fn_710050afd0(UnkGrabFx4_b61* self, UnkGrabArg_b61* arg);
void fn_710050cd98(UnkSelf* self);
void fn_710050d150(UnkGrabAttack_A3_b68* self);
extern UnkGrabArg_b61 g_unk_71726de5a0 __attribute__((visibility("hidden")));
