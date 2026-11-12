#pragma once

// UnkTU2744: functions of this translation unit that other files call (tools/tu_map.py unit 2744,
// 0x71011b020c-0x71011be0d0). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGuiRoot;
struct UnkLabelTextObject;
void fn_71011b6b5c(UnkGuiRoot* root, u32 value);
void fn_71011bab80(UnkGuiRoot* root, void* object, s32 frame);
void fn_71011b3768(UnkGuiRoot* root);
void fn_71011bab10(UnkGuiRoot* root, UnkLabelTextObject** out, s32 frame);
void fn_71011baba0(UnkGuiRoot* root, void* object, s32 frame);
