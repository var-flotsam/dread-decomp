#pragma once

#include "types.h"

// A node's position (src/game/navmesh/UnkNavMeshQueryState.cpp).
// Same layouts as src/game/navmesh/UnkNavPathNode.cpp.
// A point of a node, copied member by member.
struct UnkNavPathPoint {
    f32 x;
    f32 y;

    UnkNavPathPoint() {}
    UnkNavPathPoint(f32 x, f32 y) : x(x), y(y) {}
    UnkNavPathPoint(const UnkNavPathPoint& other) : x(other.x), y(other.y) {}
};
