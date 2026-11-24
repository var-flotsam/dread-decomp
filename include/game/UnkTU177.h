#pragma once

// UnkTU177: functions and globals of this translation unit that other files use (tools/tu_map.py unit 177,
// 0x71001587e0-0x710015beec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBlendState;
struct UnkEntry_71001cdba4;
void fn_710015a260(bool value);  // Sets the alpha write-mask flag for slots 0-7.
void fn_710015a3e0(bool value);
void fn_710015a500(u32 a, u32 b);
void fn_710015a680(u32 value);
void fn_7100159490(s32 x, s32 y, s32 width, s32 height, f32 min_depth, f32 max_depth);
bool fn_7100159c80();
void DrawIndexedFromCurrentState(u32 a, u32 b);
void fn_710015a090(s32 a, s32 b, s32 c, s32 d);
void fn_710015ab00(u32 value);
void fn_7100159800(s32 value);
bool fn_71001598c0();
UnkEntry_71001cdba4* fn_7100159aa0();  // The default set.
void fn_710015a860(u32 value);
void SetDepthCompareFunc(u32 value);
void fn_710015aa20(u32 value);
void fn_710015aa80(u32 value);
extern u8 g_unk_716e29c6a8[];
extern s32 g_unk_7101a1d7e0;
extern bool g_unk_7101a1d7e4;  // The state object's dirty flag.
extern u8 g_unk_7101a1d7e5;
extern s32 g_unk_7101a1d7ec;
extern s32 g_unk_7101a1d7f0;
extern u8 g_unk_716e29c680[];
extern bool g_unk_716e29c684;
extern u8 g_unk_716e29c690;
extern u8 g_unk_716e29c691;
extern u8 g_unk_716e29c6cc[];
extern bool g_unk_7101a1d7d8;
extern bool g_unk_7101a1d7d9;
extern s32 g_unk_7101a1d7f4;
extern s32 g_unk_7101a1d7f8;
extern s32 g_unk_7101a1d7fc;
void SetStencilOpsIfChanged(s32 a, s32 b, s32 c);
void fn_710015acf0(const UnkBlendState* desc);  // Applies desc unless the top of the blend stack locks it.
void fn_710015b160();  // Applies the top of the blend stack again.
bool PopStateStackAndApplyNewTop(bool apply);  // Pops the blend stack; whether an entry is left.
extern s32 g_unk_716e29c694;
extern s32 g_unk_716e29c240;
extern s32 g_unk_716e29c244;
void* fn_710015abd0();
void fn_7100159920();
void fn_7100159ac0();
void fn_7100159bf0(s32 a, s32 b);
void fn_710015bd90(s32 x, s32 y, s32 width, s32 height);
