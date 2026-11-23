#pragma once

#include "game/navmesh/SNavMeshPath.h"

struct UnkOffsetScratch_b65a4 {
    u8 data[0x12a88];
    UnkOffsetScratch_b65a4() { fn_71002acfe4(this); }
    ~UnkOffsetScratch_b65a4() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};
