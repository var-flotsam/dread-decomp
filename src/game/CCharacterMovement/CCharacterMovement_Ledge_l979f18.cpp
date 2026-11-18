// CCharacterMovement's unit: finds the navigation hit for `pos` (virtual 0x2e0 of the owner's movement at
// +0x1f8) and, when its edge faces down enough, fills the two requested results: from one of the lists at
// +0x428/+0x4d8/+0x588/+0x638 by the owner's facing when `use_lists` is set and the list is not empty,
// otherwise by the probe fn_710097a1fc. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU362.h"
#include "game/UnkTU363.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A result: the hit's cell, side and point, and the value of the list it came from.
struct UnkNavResult_l979f18 {
    u16 cell;
    u8 side;
    CVector2D point;  // 0x4
    f32 value;        // 0xc
};

struct UnkNavMovement_l979f18 {
    void** vtable;

    UnkNavMeshHit vf_0x2e0(const CVector3D* pos, bool flag) {
        return reinterpret_cast<UnkNavMeshHit (*)(UnkNavMovement_l979f18*, const CVector3D*, bool)>(
            vtable[0x2e0 / 8])(this, pos, flag);
    }
};

// A list of values: what fn_7100030dec reads, with its count at +0x8.
struct UnkNavList_l979f18 {
    u8 pad_0x0[0x8];
    u32 count;
    u8 pad_0xc[0xb0 - 0xc];
};

struct UnkCharacterMovement_l979f18 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x428 - 0x38];
    UnkNavList_l979f18 below_a;  // 0x428
    UnkNavList_l979f18 below_b;  // 0x4d8
    UnkNavList_l979f18 above_a;  // 0x588
    UnkNavList_l979f18 above_b;  // 0x638

    f32 Facing() { return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)).m[2]; }
};
static_assert(__builtin_offsetof(UnkCharacterMovement_l979f18, above_b) == 0x638, "layout");

// What the probe reads: the hit, the limit and the mesh.
struct UnkNavProbe_l979f18 {
    UnkNavMeshHit* hit;
    f32* limit;
    UnkNavMesh** mesh;
};
void fn_710097a1fc(UnkNavProbe_l979f18* probe, bool facing_away, UnkNavResult_l979f18* out, bool* found);

static inline UnkNavMeshManager* NavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

// A message record (constructed by fn_71002878dc, destroyed by fn_7100287954).
struct UnkNavMsg_l979f18 {
    u8 data[0x10];
    UnkNavMsg_l979f18() { fn_71002878dc(reinterpret_cast<UnkMsg_b152*>(this)); }
    ~UnkNavMsg_l979f18() { fn_7100287954(reinterpret_cast<UnkMsg_b152*>(this)); }
};

static inline void Fill(UnkNavResult_l979f18* out, const UnkNavMeshHit& hit, UnkNavList_l979f18* list) {
    u16 cell = hit.cell;
    u8 side = hit.side;
    out->side = side;
    out->cell = cell;
    out->point.x = hit.point.x;
    out->point.y = hit.point.y;
    out->value = fn_7100030dec(reinterpret_cast<UnkFloatList_b46a1*>(list));
}

// Fills the requested result from `list` and reports it found.
static inline void Report(UnkNavResult_l979f18* out, bool* found, const UnkNavMeshHit& hit, UnkNavList_l979f18* list) {
    if (out)
        Fill(out, hit, list);
    if (found)
        *found = true;
}

bool fn_7100979f18(UnkCharacterMovement_l979f18* self, const CVector3D* pos, bool use_lists, UnkNavResult_l979f18* out_a,
                   bool* found_a, UnkNavResult_l979f18* out_b, bool* found_b) {
    if (!out_a && !out_b)
        return false;
    UnkNavMesh* mesh = NavManager()->navmesh;
    auto* movement = static_cast<UnkNavMovement_l979f18*>(self->owner->field_0x1f8);
    if (!mesh || !movement)
        return false;
    f32 facing = self->Facing();
    f32 limit = -0.5f;
    UnkNavMeshHit hit = movement->vf_0x2e0(pos, use_lists);
    if (!hit.IsValid())
        return false;
    UnkNavMeshEdge* edge = fn_71002afbe4(fn_710027a328(mesh, hit.cell), &hit.side);
    if (!(edge->field_0x10 * -0.0f - edge->field_0xc <= -0.5f))
        return false;

    UnkNavProbe_l979f18 probe = {&hit, &limit, &mesh};
    // Two records the original constructs and destroys around the probes without reading them.
    UnkNavMsg_l979f18 first;
    UnkNavMsg_l979f18 second;
    if (out_b) {
        // Faithful: a list found here fills out_a and found_a, as the original does.
        if (use_lists && self->Facing() < 0.0f && self->below_b.count)
            Report(out_a, found_a, hit, &self->below_b);
        else if (use_lists && self->Facing() > 0.0f && self->above_b.count)
            Report(out_a, found_a, hit, &self->above_b);
        else
            fn_710097a1fc(&probe, !(facing > 0.0f), out_b, found_b);
    }
    if (out_a) {
        if (use_lists && self->Facing() < 0.0f && self->below_a.count)
            Report(out_a, found_a, hit, &self->below_a);
        else if (use_lists && self->Facing() > 0.0f && self->above_a.count)
            Report(out_a, found_a, hit, &self->above_a);
        else
            fn_710097a1fc(&probe, facing > 0.0f, out_a, found_a);
    }
    return true;
}
