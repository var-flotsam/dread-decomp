#pragma once

// The mesh of a dashed line or a quad (src/game/UnkDashedPathMesh.cpp fills it; the minimap,
// src/game/CMinimapData.cpp, draws with it): its vertices, the triangles' indices and the 2D
// bounds of the vertices. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A vertex: a position and its depth (UnkVec3f of game/UnkXformNode.h, copied word by word).
using UnkMeshVertex = UnkVec3f;

struct UnkDashMesh {
    base::global::CRntVector<UnkMeshVertex> vertices;
    base::global::CRntVector<u16> indices;
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};
static_assert(__builtin_offsetof(UnkDashMesh, min_x) == 0x50, "UnkDashMesh placeholder layout");
