// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): the out-of-line element accessor
// (base/global/CRntVector.h, GrowAt) of a vector of navigation-mesh hits, the 12-byte records of
// src/game/navmesh/CNavMesh.cpp (constructed by fn_7100287598, destroyed by fn_710028760c).
// Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkTU362.h"
#include "types.h"

using base::math::CVector2D;

UnkNavMeshHit* fn_710046fb88(base::global::CRntVector<UnkNavMeshHit>* hits, u32 index) {
    return &hits->GrowAt(index);
}
