#pragma once

// UnkTU1102: functions of this translation unit that other files call (tools/tu_map.py unit 1102,
// 0x71006e5f24-0x71006ebf9c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkXParasiteAI_A3_b75;
struct UnkXParasiteAI_b64a4;
struct UnkXParasiteGrab_xpa;
struct UnkXParasiteMode_A3_b75;
void fn_71006e9384(UnkXParasiteAI_A3_b75* self, const UnkXParasiteMode_A3_b75* mode);
void* fn_71006eb114(void* owner);  // refl_FindTyped of "SMARTOBJECT".
void fn_71006e892c(UnkXParasiteGrab_xpa* self);
void fn_71006ea3fc(UnkXParasiteAI_b64a4* self, bool flag);
void* fn_71006eb1f0(void* h);
