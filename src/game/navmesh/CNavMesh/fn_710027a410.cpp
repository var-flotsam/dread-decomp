// In navmesh::CNavMesh's unit: the mesh's vertex `index`, from its array of 8-byte points at +0x2b8
// (its callers, src/game/UnkNavMeshCellLoad.cpp and src/game_hi/fn_7100ebfa7c.cpp, read it as a 2D
// point).
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct Vec2;
using Mesh = UnkNavMesh;

struct UnkNavMeshVertices {
    u8 pad_0x0[0x2b8];
    u64* vertices;
};

Vec2* fn_710027a410(Mesh* mesh, u16 index) {
    return reinterpret_cast<Vec2*>(&reinterpret_cast<UnkNavMeshVertices*>(mesh)->vertices[index]);
}
