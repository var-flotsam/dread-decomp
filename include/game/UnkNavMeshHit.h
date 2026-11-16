#pragma once

// A navigation-mesh hit (12 bytes): the cell, the side of the cell and the point. The same record as
// UnkElem_7100286624 (src/game_lo/fn_7100286624.cpp). Constructed empty by fn_7100287598 or from a packed
// cell and side and a point by fn_71002875bc, destroyed by fn_710028760c (include/game/UnkTU362.h).
// Names are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU362.h"
#include "types.h"

struct UnkElem_7100286624;

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    base::math::CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const u32* cell_side, const base::math::CVector2D* at) { fn_71002875bc(this, cell_side, at); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");
