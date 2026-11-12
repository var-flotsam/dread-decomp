#pragma once

// UnkTU2509: functions of this translation unit that other files call (tools/tu_map.py unit 2509,
// 0x7100d5134c-0x7100d5761c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkShapeHit_A5_b68;
struct UnkMatrix44;
struct UnkShapeFilter_ovb0107;
struct UnkShapeTestResult_ovb0107;
struct UnkShapeTester_A5_b68;
void fn_7100d5134c(UnkShapeTester_A5_b68* self);
UnkShapeTestResult_ovb0107 fn_7100d51368(UnkShapeTester_A5_b68* tester, void* owner, void* shape, void* other, const UnkMatrix44& transform, UnkShapeFilter_ovb0107* filter, u32);
UnkShapeHit_A5_b68 fn_7100d572f8(UnkShapeTester_A5_b68* tester, void* shape, const base::math::CVector2D* pos, const base::math::CVector3D* offset, u64 flags);
