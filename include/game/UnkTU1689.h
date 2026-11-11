#pragma once

// UnkTU1689: functions of this translation unit that other files call (tools/tu_map.py unit 1689,
// 0x7100b37360-0x7100b49730). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkIdValuesArray;
struct UnkMapMenuControllerClose;
void fn_7100b3e5ac(UnkMapMenuControllerClose* self, s32 entry, u32 mode);
void fn_7100b48af4(UnkIdValuesArray* self, u32 capacity);
