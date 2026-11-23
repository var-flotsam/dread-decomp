#pragma once

#include "base/math/CVector.h"
#include "types.h"

// A point of the marker's path, handed back in memory (a user-provided destructor makes it so).
// A point of the path, handed back in memory (as src/game/CMinimapData.cpp declares it).
struct UnkMinimapPathPoint {
    base::math::CVector3D v;
    ~UnkMinimapPathPoint() {}
};
