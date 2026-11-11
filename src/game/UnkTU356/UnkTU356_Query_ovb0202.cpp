// Unit 356 of tools/tu_map.py (navmesh query state, src/game/navmesh/UnkNavMeshQueryState.cpp): the
// query state's reset, the search driver that runs it to the end and builds the path, and the one that
// hands each opened node's cell to a callback until it answers. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/UnkVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU357.h"
#include "game/UnkTU358.h"
#include "types.h"

struct UnkNavQuery;
struct UnkStruct_7100286624;
struct UnkNavPathNode;

struct UnkNavPathNode_b0202 {
    u8 data[0x128];
};

struct UnkNavHit_b0202 {
    u16 cell;
    u8 side;
    f32 x;
    f32 y;
};

// A count followed by 1200 pointers: the query keeps three of them and then the node array's count.
struct UnkNavList_b0202 {
    u32 count;
    u8 storage[0x2580];
};

// The third list and the node array's count after it.
struct UnkNavListNodes_b0202 {
    u32 count;
    u8 storage[0x2580];
    u32 node_count;  // 0x2584
    UnkNavPathNode_b0202 nodes[1200];
};

struct UnkNavQueryState_b0202 {
    void* mesh;  // 0x0
    u32 open_count;  // 0x8
    u8 open_storage[0x2580];
    u32 second_count;  // 0x258c
    u8 second_storage[0x2580];
    UnkNavListNodes_b0202 third;  // 0x4b10
    u32 status;  // 0x5dc18
    u8 pad_0x5dc1c[4];
    u8 query[0x1b8];  // 0x5dc20
    u64 field_0x5ddd8;
    UnkNavHit_b0202 hit;  // 0x5dde0
    u32 field_0x5ddec;

};
static_assert(__builtin_offsetof(UnkNavQueryState_b0202, third) == 0x4b10, "state");
static_assert(__builtin_offsetof(UnkNavQueryState_b0202, status) == 0x5dc18, "state");
static_assert(__builtin_offsetof(UnkNavQueryState_b0202, field_0x5ddec) == 0x5ddec, "state");
static_assert(sizeof(UnkNavQueryState_b0202) == 0x5ddf0, "state");

// The reset: destroys the query, the nodes and clears the lists.
void fn_7100281134(UnkNavQueryState_b0202* self) {
    fn_7100286624(reinterpret_cast<UnkStruct_7100286624*>(self->query));
    for (u64 i = 0; i < self->third.node_count; ++i)
        fn_71002872f4(reinterpret_cast<UnkNavPathNode*>(&self->third.nodes[i]));
    self->third.node_count = 0;
    self->third.count = 0;
    self->second_count = 0;
    self->open_count = 0;
}

// The step loop: opens nodes (fn_71002814e8) until it has nothing left to do, then, for a finished
// search, asks fn_7100281a24 to build the path in `path` and records the goal's cell in it.
struct UnkNavMeshPathIds;
bool fn_71002814e8(UnkNavQueryState_b0202* self);
void fn_7100281290(UnkNavQueryState_b0202* self);
bool fn_7100281a24(UnkNavQueryState_b0202* self, UnkNavPathNode* node, UnkNavMeshPathIds* path);
void fn_71002adf48(UnkNavMeshPathIds* self, s16 id);

bool fn_71002811b4(UnkNavQueryState_b0202* self, void* unused, UnkNavMeshPathIds* path) {
    fn_7100281290(self);
    while (fn_71002814e8(self)) {
    }
    UnkNavPathNode* node;
    if (self->status == 3) {
        node = self->open_count ? *reinterpret_cast<UnkNavPathNode**>(&self->open_storage[0]) : nullptr;
    } else if (self->status == 4) {
        if (*reinterpret_cast<s16*>(self->query + 0x78) != -1)
            return false;
        node = nullptr;
    } else {
        return false;
    }
    if (!fn_7100281a24(self, node, path))
        return false;
    u16 cell = fn_710027a41c(reinterpret_cast<UnkNavMesh*>(self->mesh),
                             reinterpret_cast<base::math::CVector2D*>(reinterpret_cast<u8*>(self) + 0x5dca4), 1);
    if ((u16)~cell != 0)
        fn_71002adf48(path, cell);
    return true;
}

// A callback: a context and a function over a cell and the query's result.
struct UnkNavCellCallback_b0202 {
    void* ctx;
    void* (*fn)(void* ctx, UnkNavMeshCell* cell, void* result);
};

struct UnkNavQueryResult_b46a1;
void fn_71002869a8(UnkNavQueryResult_b46a1* result);

static inline UnkNavMeshManager* GetRegistry() {
    UnkNavMeshManager* registry = g_unk_71726a4510;
    if (!registry) {
        registry = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(registry);
        g_unk_71726a4510 = registry;
    }
    return registry;
}

// Runs the search, calling `callback` with the cell of each node it opens until one answers.
void* fn_7100282b3c(UnkNavQueryState_b0202* self, void* unused, UnkNavCellCallback_b0202* callback) {
    fn_7100281290(self);
    u8* query = self->query;
    if (*(reinterpret_cast<u8*>(self) + 0x5dca0))
        fn_71002869a8(reinterpret_cast<UnkNavQueryResult_b46a1*>(query));
    while (fn_71002814e8(self)) {
        if (!self->open_count)
            break;
        u8* node = *reinterpret_cast<u8**>(&self->open_storage[0]);
        if (!node)
            break;
        UnkNavMeshManager* registry = GetRegistry();
        u16 cell = *reinterpret_cast<u16*>(node + 0x100);
        if (cell == 0xffff)
            continue;
        UnkNavMeshCell* mesh_cell = fn_710027a328(registry->navmesh, cell);
        if (callback->fn) {
            if (void* r = callback->fn(callback->ctx, mesh_cell, query))
                return r;
        }
    }
    return nullptr;
}
