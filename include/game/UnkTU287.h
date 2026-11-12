#pragma once

// UnkTU287: functions of this translation unit that other files call (tools/tu_map.py unit 287,
// 0x710020a18c-0x710020ae7c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkStruct_710020a5a0;
struct UnkMinimapVec3;
void fn_710020ad90(void* self, s32 value);
void fn_710020a4c4(void* node, const UnkMinimapVec3& pos);
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
