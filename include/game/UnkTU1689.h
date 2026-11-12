#pragma once

// UnkTU1689: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1689,
// 0x7100b37360-0x7100b49730). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdValuesArray;
struct UnkMapMenuControllerClose;
void fn_7100b3e5ac(UnkMapMenuControllerClose* self, s32 entry, u32 mode);
void fn_7100b48af4(UnkIdValuesArray* self, u32 capacity);
extern void* g_unk_7101a9c8e0[];  // The base's vtable.
extern void* g_unk_7101a9ca40[];
extern void* g_unk_7101a9cb48[];  // The object's vtable.
extern base::global::CStrId g_unk_7172763d98 __attribute__((visibility("hidden")));
