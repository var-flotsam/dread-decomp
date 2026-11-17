#pragma once

// CDemolitionBlockComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1211,
// 0x71007afa60-0x71007b1bac). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"
void fn_71007b1528(char* self);
void fn_71007b1928(void*, void*);  // Moves the entry at the second pointer into the first.

template <u32 IdOffset>
struct UnkActivatable_b44a2;
void fn_71007b00cc(UnkActivatable_b44a2<0x48>* self);  // The activatable component's destructor.
extern void* g_unk_7101a70380[];
