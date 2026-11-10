// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): the out-of-line element accessor
// (base/UnkVector.h, GrowAt) of a vector of navigation-mesh hits, the 12-byte records of
// src/game/navmesh/CNavMesh.cpp (constructed by fn_7100287598, destroyed by fn_710028760c).
// Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

// From src/game/navmesh/CNavMesh.cpp (the same definitions).
struct UnkNavMeshHit;
struct UnkElem_7100286624;  // the same 12-byte record (src/game_lo/fn_7100286624.cpp)
void fn_7100287598(UnkNavMeshHit* self);
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");

UnkNavMeshHit* fn_710046fb88(base::UnkVector<UnkNavMeshHit>* hits, u32 index) {
    return &hits->GrowAt(index);
}
