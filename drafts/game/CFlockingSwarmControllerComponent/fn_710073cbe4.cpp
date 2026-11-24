// CFlockingSwarmControllerComponent unit: plans a navigation path for the swarm between two points,
// refusing when either point is off the mesh or when neither end's cell lies on the side the swarm
// wants. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU357.h"
#include "game/UnkTU367.h"
#include "types.h"

using base::math::CVector2D;

struct UnkStruct_7100286798;
struct UnkActor_7100286798;
struct UnkStruct_7100286624;

bool fn_7100ebfa6c(UnkNavMeshManager* self, UnkNavQuery* query, void* path);

// The navigation query as this function fills it.
struct UnkSwarmNavQuery_l73cbe4 {
    u8 pad_0x0[0x154];
    u32 field_0x154;
    u8 pad_0x158[0x54];
    u32 field_0x1ac;
    u8 pad_0x1b0[0x8];
};
static_assert(sizeof(UnkSwarmNavQuery_l73cbe4) == 0x1b8, "UnkSwarmNavQuery_l73cbe4 placeholder layout");

struct UnkSwarmNavParams_l73cbe4 {
    u8 pad_0x0[0x58];
    u8 field_0x58;  // nonzero: the swarm wants cells with flag 2
    u8 pad_0x59[0x3ff];
    u32 field_0x458;
};

struct UnkSwarmComponent_l73cbe4 {
    u8 pad_0x0[0x30];
    UnkActor_7100286798* owner;
    u8 pad_0x38[0x2f0];
    UnkSwarmNavParams_l73cbe4* params;  // +0x328
};

static inline UnkNavMeshManager* NavMeshManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline bool CellSide(UnkNavMesh* mesh, u16 cell) {
    return (fn_71002b009c(fn_710027a328(mesh, cell)) & 2) != 0;
}

bool fn_710073cbe4(UnkSwarmComponent_l73cbe4* self, const CVector2D* from, const CVector2D* to, void* path,
                   bool any_side) {
    UnkNavMeshManager* manager = NavMeshManager();
    UnkNavMesh* mesh = manager->navmesh;
    u16 from_cell = CNavMesh_FindCellIndexAtPoint(mesh, from, 0);
    if (from_cell == 0xffff)
        return false;
    u16 to_cell = CNavMesh_FindCellIndexAtPoint(mesh, to, 0);
    if (to_cell == 0xffff)
        return false;

    UnkSwarmNavQuery_l73cbe4 query;
    UnkNavQuery* q = reinterpret_cast<UnkNavQuery*>(&query);
    fn_710028659c(q);
    fn_7100286920(q, from, from_cell, 0xff);
    fn_7100286964(q, to, to_cell, nullptr);
    fn_7100286798(reinterpret_cast<UnkStruct_7100286798*>(&query), self->owner);
    query.field_0x1ac = self->params->field_0x458;

    bool result = false;
    bool plan = true;
    if (!any_side) {
        bool from_side = CellSide(mesh, from_cell);
        bool want = self->params->field_0x58 != 0;
        if (want == from_side)
            query.field_0x154 = 2;
        else if (want != CellSide(mesh, to_cell))
            plan = false;
    }
    if (plan)
        result = fn_7100ebfa6c(manager, q, path);
    fn_7100286624(reinterpret_cast<UnkStruct_7100286624*>(&query));
    return result;
}
