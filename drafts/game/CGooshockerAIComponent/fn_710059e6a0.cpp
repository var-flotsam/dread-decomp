// CGooshockerAIComponent unit (tools/tu_map.py unit 854): while the owner's movement (+0x218) is in
// state 1 and its navigation-mesh side leads to a cell other than `cell`, probes the collision tree
// along a segment ahead of the owner (twice its radius, along its matrix's second column); on a hit
// with time left (+0x148c8) it runs fn_710059e8e0. Always answers true. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU362.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTree;
struct UnkBox;

// ---- the collision-tree segment query (as src/game/CCharacterMovement.cpp spells it) ----

struct UnkHitRef_l59e6a0 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l59e6a0() : ptr(nullptr) {}
    ~UnkHitRef_l59e6a0() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l59e6a0 {
    f32 x, y, z;
    UnkHitVec_l59e6a0(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l59e6a0 {
    UnkHitRef_l59e6a0 object;
    UnkHitRef_l59e6a0 field_0x8;
    s32 id;
    UnkHitVec_l59e6a0 point;
    UnkHitVec_l59e6a0 field_0x20;
    UnkHitVec_l59e6a0 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l59e6a0()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkSegmentHitQuery_l59e6a0 {
    void** vtable;
    UnkCollisionHit_l59e6a0 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_l59e6a0(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_l59e6a0() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkHitSegment_l59e6a0 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_l59e6a0(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// ---- the component ----

struct UnkGooshockerNav_l59e6a0 {
    void** vtable;
    UnkNavMeshHit vf_0x2d8() {
        return reinterpret_cast<UnkNavMeshHit (*)(UnkGooshockerNav_l59e6a0*)>(vtable[0x2d8 / 8])(this);
    }
};

struct UnkGooshockerAI_l59e6a0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x148c8 - 0x38];
    f32 field_0x148c8;
};

void fn_710059e8e0(UnkGooshockerAI_l59e6a0* self);

bool fn_710059e6a0(UnkGooshockerAI_l59e6a0* self, u16 cell) {
    void* movement = self->owner->field_0x218;
    if (!movement || fn_71010cc3a4(movement) != 1)
        return true;
    auto* nav = static_cast<UnkGooshockerNav_l59e6a0*>(self->owner->field_0x218);
    if (!nav)
        return true;
    bool blocked = false;
    {
        UnkNavMeshHit hit = nav->vf_0x2d8();
        u16 hit_cell = hit.cell;
        if (hit_cell != 0xffff && hit.side != 0xff) {
            u8 side = hit.side;
            UnkNavMeshManager* manager = g_unk_71726a4510;
            if (!manager) {
                manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
                fn_7100ebf278(manager);
                g_unk_71726a4510 = manager;
            }
            if (manager->navmesh &&
                fn_71002afbe4(fn_710027a328(manager->navmesh, hit_cell), &side)->field_0x2 != cell) {
                // A segment ahead of the owner: twice its radius along its matrix's second column.
                UnkActor* owner = self->owner;
                CVector2D start = {owner->field_0x44, owner->field_0x54};
                const UnkMatrix44& xf = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
                owner = self->owner;
                f32 reach = owner->field_0x37c + owner->field_0x37c;
                CVector2D end = {start.x - xf.m[1] * reach, start.y - xf.m[5] * reach};
                UnkHitSegment_l59e6a0 segment(start, end);
                UnkSegmentHitQuery_l59e6a0 query(CVector3D{owner->field_0x44, owner->field_0x54, owner->field_0x64});
                UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
                blocked = fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2);
            }
        }
    }
    if (blocked && self->field_0x148c8 > 0.0f)
        fn_710059e8e0(self);
    return true;
}
