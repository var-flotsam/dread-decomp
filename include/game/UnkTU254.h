#pragma once

// UnkTU254: functions of this translation unit that other files call (tools/tu_map.py unit 254,
// 0x71001c33cc-0x71001c383c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "types.h"

struct UnkMinimapView;
struct UnkObject_71001c3504;
void fn_71001c34f4(void* target, f32 value);
void fn_71001c3770(UnkMinimapView* view, s32);
void fn_71001c36c0(UnkMinimapView* view, const base::math::CVector2D* pos, const base::math::CVector2D* size);
// Sets the string id of an object's lazily created part (src/game_s1/fn_71001c3504.cpp).
void fn_71001c3504(UnkObject_71001c3504* self, const base::UnkStrIdMember* id);
