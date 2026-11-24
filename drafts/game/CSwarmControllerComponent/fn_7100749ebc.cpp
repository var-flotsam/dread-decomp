// CSwarmControllerComponent's unit (1163): whether a swarm member and the next member of its group stand
// in navigation-mesh cells that are both open (neither cell's +0x68 timer says otherwise).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A swarm member (its vf_0x18 gives its position).
struct UnkSwarmMember_l749ebc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* GetPosition();  // vf_0x18
};

// A group of members.
struct UnkSwarmGroup_l749ebc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Count();                            // vf_0x28
    virtual UnkSwarmMember_l749ebc* At(u32 index);  // vf_0x30
};

// A member's place: its group and its index in the group.
struct UnkSwarmSlot_l749ebc {
    u32 group;
    u32 index;
};

struct UnkSwarmController_l749ebc;
struct UnkSwarmControllerVtable_l749ebc {
    void* pad_0x0[0x1a8 / 8];
    UnkSwarmGroup_l749ebc* (*GetGroup)(UnkSwarmController_l749ebc* self, u32 group);  // 0x1a8
};
struct UnkSwarmController_l749ebc {
    UnkSwarmControllerVtable_l749ebc* vtable;
};

// The cell's state object (+0x68): its timer at +0x18.
struct UnkNavCellState_l749ebc {
    u8 pad_0x0[0x18];
    u8 timer[0x8];  // 0x18
};
struct UnkNavMeshCellState_l749ebc {
    u8 pad_0x0[0x68];
    UnkNavCellState_l749ebc* state;  // 0x68
};

static inline UnkNavMeshManager* NavMeshManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline UnkSwarmGroup_l749ebc* Group(UnkSwarmController_l749ebc* self, u32 group) {
    return self->vtable->GetGroup(self, group);
}

// The slot after `slot` in its group, wrapping around.
static inline UnkSwarmSlot_l749ebc NextSlot(UnkSwarmController_l749ebc* self, const UnkSwarmSlot_l749ebc& slot) {
    UnkSwarmSlot_l749ebc next;
    if (UnkSwarmGroup_l749ebc* group = Group(self, slot.group)) {
        u32 count = group->Count();
        next = slot;
        u32 following = next.index == count - 1 ? 0 : next.index + 1;
        next.index = count != 0 ? following : next.index;
    } else {
        next = slot;
    }
    return next;
}

static inline UnkSwarmMember_l749ebc* Member(UnkSwarmController_l749ebc* self, const UnkSwarmSlot_l749ebc& slot) {
    UnkSwarmGroup_l749ebc* group = Group(self, slot.group);
    if (group && slot.index < group->Count())
        return group->At(slot.index);
    return nullptr;
}

static inline bool IsClosed(UnkNavMeshCell* cell) {
    return fn_710002fb70(reinterpret_cast<UnkNavMeshCellState_l749ebc*>(cell)->state->timer);
}

bool fn_7100749ebc(UnkSwarmController_l749ebc* self, const UnkSwarmSlot_l749ebc* slot) {
    UnkSwarmSlot_l749ebc next = NextSlot(self, *slot);
    UnkSwarmMember_l749ebc* member = Member(self, *slot);
    UnkSwarmMember_l749ebc* neighbour = Member(self, next);

    CVector2D from;
    fn_710007b430(&from, member->GetPosition());
    CVector2D to;
    fn_710007b430(&to, neighbour->GetPosition());

    UnkNavMesh* mesh = NavMeshManager()->navmesh;
    u16 fromCell = CNavMesh_FindCellIndexAtPoint(mesh, &from, 0);
    u16 toCell = CNavMesh_FindCellIndexAtPoint(mesh, &to, 0);
    if (fromCell == 0xffff || toCell == 0xffff)
        return false;
    UnkNavMeshCell* a = fn_710027a328(mesh, fromCell);
    UnkNavMeshCell* b = fn_710027a328(mesh, toCell);
    return !IsClosed(a) && !IsClosed(b);
}
