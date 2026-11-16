#pragma once

// CNavMesh: functions of this translation unit that other files call (tools/tu_map.py unit 355,
// 0x7100274548-0x71002810b0). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkNavMesh;
struct UnkNavLoadCell;
struct UnkOwner_710027a41c;
bool fn_710027a5f0(UnkOwner_710027a41c* self, const base::math::CVector2D* point, u16 index);
void fn_7100279600(base::global::CRntVector<UnkNavLoadCell>* self, u32 capacity);  // The cell array's reserve.
void fn_710027ebe4(base::global::CRntVector<UnkNavMeshEdge>* self, const base::global::CRntVector<UnkNavMeshEdge>* other);
u32 fn_710027a320(UnkNavMesh* mesh);
