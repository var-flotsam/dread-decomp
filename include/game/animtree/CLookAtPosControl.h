#pragma once

// animtree::CLookAtPosControl: functions of this translation unit that other files call (tools/tu_map.py
// unit 418, 0x71002e6470-0x71002e6d20). One declaration per function: include this instead of declaring
// one. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkLookAtPosControl_b69;
// Sets the position to look at and the time to turn to it
// (src/game/animtree/CLookAtPosControl/CLookAtPosControl_SetTarget_A2_b69.cpp).
void CLookAtPosControl_SetTargetPos(UnkLookAtPosControl_b69* self, const base::math::CVector3D& pos, f32 time);
