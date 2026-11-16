#pragma once

// UnkTU362: functions of this translation unit that other files call (tools/tu_map.py unit 362,
// 0x7100287598-0x71002878dc). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

struct UnkElem_7100286624;
struct UnkNavMeshHit;
void fn_710028760c(UnkElem_7100286624* self);
void fn_7100287598(UnkNavMeshHit* self);
// Constructs a hit from a packed cell and side and a point.
void fn_71002875bc(UnkNavMeshHit* self, const u32* cell_side, const base::math::CVector2D* at);
