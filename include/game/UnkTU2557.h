#pragma once

// UnkTU2557: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2557,
// 0x7100e0f028-0x7100e140c0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkReflType;
struct UnkLoadingScreenData;
struct UnkLoadingScreen;
void fn_7100e0f89c(UnkLoadingScreen* self);  // Constructor.
void fn_7100e0f028();
void fn_7100e1389c() __attribute__((visibility("hidden")));
void fn_7100e138a8() __attribute__((visibility("hidden")));
extern void* g_unk_71727c0708;  // A render object the loading screen keeps (pool 0).
extern char g_unk_71727c0828;
extern UnkLoadingScreenData g_unk_71727c0870 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727c0710;
