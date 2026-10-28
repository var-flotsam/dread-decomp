#pragma once

// The navigation-mesh manager singleton (reached through the GOT): a 0x62ec8-byte object the AI
// sources create on first use with fn_71000839a0(0x62ec8, 0x3f) and its constructor fn_7100ebf278
// (game_hi/fn_7100ebfa7c.cpp, its unit). It holds the current navigation mesh at +0x0 (the object
// the functions of the navmesh::CNavMesh unit take: cells and vertices for game_hi/fn_7100ebfa7c,
// the per-entity 0x70-byte entries fn_710027a33c grows at its +0x290 for game_s2/fn_7100569f34,
// and the key/value table at its +0x338 that game_lo/fn_7100286798 reads), a 0x5ddf0-byte query
// state (fn_71002810b0 constructs it), a 0x5038-byte reachability tester (fn_71002b4934 constructs
// it; fn_7100ec1454 asks it whether two entries can reach each other for game_aca57c/fn_7100ad0294)
// and, at the end, a registry of navmesh::CDynamicSmartLinkGroup objects keyed by string id
// (fn_7100ec0d20 looks one up for game_s2/fn_7100413ef8, creating it on demand): a dictionary
// whose 0x18-byte nodes come from the block pool after it (the UnkPoolList shape of
// base/UnkCloneHelpers.h; fn_7100ebf2fc constructs it with 100 nodes). Names and layouts are
// placeholders; only what the hand-written files touch is spelled out.

#include "types.h"

namespace base {
struct UnkAllocator;
namespace math {
struct CVector2D;
}
}  // namespace base

struct UnkNavMesh;  // the navigation mesh (probably navmesh::CNavMesh); opaque here

struct UnkNavMeshState_0x5ddf0 {  // the query state, constructed by fn_71002810b0
    u8 pad_0x0[0x5ddf0];
};
struct UnkNavMeshReach_0x5038 {  // the reachability tester, constructed by fn_71002b4934
    u8 pad_0x0[0x5038];
};
struct UnkNavMeshNodePool_0x70 {  // the registry's node pool, constructed by fn_7100ebf2fc
    u8 pad_0x0[0x70];
};

struct UnkNavMeshManager {
    UnkNavMesh* navmesh;            // the current mesh; null until one is set
    UnkNavMeshState_0x5ddf0 state;  // +0x8
    UnkNavMeshReach_0x5038 reach;   // +0x5ddf8
    void* field_0x62e30;            // the smart-link group registry: three pointers,
    void* field_0x62e38;            // a growth step and allocation flags, then its allocator
    void* field_0x62e40;
    u32 field_0x62e48;              // 10
    u32 field_0x62e4c;              // 0x57
    base::UnkAllocator* allocator;  // +0x62e50, the default allocator
    UnkNavMeshNodePool_0x70 pool;   // +0x62e58
};
static_assert(__builtin_offsetof(UnkNavMeshManager, reach) == 0x5ddf8, "UnkNavMeshManager placeholder layout");
static_assert(__builtin_offsetof(UnkNavMeshManager, field_0x62e30) == 0x62e30, "UnkNavMeshManager placeholder layout");
static_assert(__builtin_offsetof(UnkNavMeshManager, pool) == 0x62e58, "UnkNavMeshManager placeholder layout");
static_assert(sizeof(UnkNavMeshManager) == 0x62ec8, "UnkNavMeshManager placeholder layout");

extern UnkNavMeshManager* g_unk_71726a4510;  // the instance; null until first use
void fn_7100ebf278(UnkNavMeshManager* self);  // constructor (game_hi/fn_7100ebfa7c.cpp)

// The index of the mesh's cell (0x70-byte entry at +0x290) that contains `point`, found through the
// bounding-box tree at +0x400 when there is one, if the cell accepts `flags`; 0xffff otherwise.
// Defined in src/game/navmesh/CNavMesh/fn_710027a41c.cpp.
u16 fn_710027a41c(UnkNavMesh* self, const base::math::CVector2D* point, u32 flags);
