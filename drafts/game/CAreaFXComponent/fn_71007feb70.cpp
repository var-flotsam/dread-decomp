// CAreaFXComponent unit (tools/tu_map.py unit 1263): the owner's position moved to its anchor (the point the
// model updater gives for the id table's anchor id), then back along the sweep's hit normal when its collision
// shape, swept from the owner to there, hits anything; when the result is not the owner's position the owner
// is moved there. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool SweepShapeAgainstCollisionTree(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

extern void* g_unk_7101a23590[];

struct UnkHitObj_l7feb70 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l7feb70 {
    UnkHitObj_l7feb70* ptr;

    UnkHitRef_l7feb70() : ptr(nullptr) {}
    ~UnkHitRef_l7feb70() {
        if (UnkHitObj_l7feb70* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l7feb70 {
    f32 x, y, z;
    UnkHitVec_l7feb70(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l7feb70 {
    UnkHitRef_l7feb70 object;
    UnkHitRef_l7feb70 field_0x8;
    s32 id;
    UnkHitVec_l7feb70 point;
    UnkHitVec_l7feb70 field_0x20;
    UnkHitVec_l7feb70 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l7feb70()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query the sweep fills.
struct UnkSweepQuery_l7feb70 {
    void** vtable;
    UnkCollisionHit_l7feb70 hit;
    bool field_0x50;

    UnkSweepQuery_l7feb70() {
        field_0x50 = false;
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkSweepQuery_l7feb70() { vtable = g_unk_7101a20ed8 + 2; }
};

// A sweep offset: the x and y of a 3D one.
struct UnkSweepOffset_l7feb70 {
    CVector2D v;

    explicit UnkSweepOffset_l7feb70(const CVector3D& offset) { fn_710007b430(&v, &offset); }
};

// What fn_7100976588 returns: the collider whose shape is at +0x158.
struct UnkCollider_l7feb70 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

struct UnkFlaggedMask_l7feb70 {
    u8 pad_0x0[0x3cc];
    u32 mask;
};

struct UnkAreaFXPart_l7feb70 {
    u8 pad_0x0[0x16e6];
    u8 moved;  // 0x16e6
};

struct UnkAnchorIdHolder_l7feb70 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkAreaFXPos_l7feb70 {
    f32 x, y, z;
};

struct UnkAreaFXOwner_l7feb70;

struct UnkAreaFXOwnerVt_l7feb70 {
    void* pad_0x0[26];
    void (*SetPosition)(UnkAreaFXOwner_l7feb70* self, const UnkAreaFXPos_l7feb70* pos, bool flag);  // 0xd0
};

struct UnkAreaFXOwner_l7feb70 {
    const UnkAreaFXOwnerVt_l7feb70* vt;
    u8 pad_0x8[0x30];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x1b0];
    UnkFlaggedMask_l7feb70* flagged;  // 0x218
    u8 pad_0x220[0x8];
    void* updater;  // 0x228
    u8 pad_0x230[0x10];
    UnkAreaFXPart_l7feb70* part;  // 0x240

    UnkAreaFXPos_l7feb70 Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};
static_assert(__builtin_offsetof(UnkAreaFXOwner_l7feb70, part) == 0x240, "UnkAreaFXOwner layout");

struct UnkAreaFXSelf_l7feb70 {
    u8 pad_0x0[0x30];
    UnkAreaFXOwner_l7feb70* owner;  // 0x30
};

static inline u64 AnchorId_l7feb70() {
    return (*reinterpret_cast<UnkAnchorIdHolder_l7feb70* const*>(GameManagerIdTable().pad_0x0 + 0xd28))->id;
}

// The point the model updater gives for the anchor id.
static inline bool GetAnchor_l7feb70(void* updater, UnkAreaFXPos_l7feb70* out) {
    u64 id = AnchorId_l7feb70();
    return fn_710117364c(updater, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(out));
}

static inline CVector3D Offset_l7feb70(const UnkAreaFXPos_l7feb70& to, const UnkAreaFXPos_l7feb70& from) {
    return CVector3D{to.x - from.x, to.y - from.y, to.z - from.z};
}

// Whether v is within a thousandth of target.
static inline bool IsNear_l7feb70(f32 target, f32 v) {
    return target + 0.001f >= v && target - 0.001f <= v;
}

void fn_71007feb70(UnkAreaFXSelf_l7feb70* self, UnkAreaFXPos_l7feb70* pos) {
    void* updater = self->owner->updater;
    UnkFlaggedMask_l7feb70* flagged = self->owner->flagged;
    *pos = self->owner->Position();
    if (!updater)
        return;
    bool found = GetAnchor_l7feb70(updater, pos);
    if (!flagged || !found)
        return;
    UnkCollider_l7feb70* collider =
        reinterpret_cast<UnkCollider_l7feb70*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(flagged)));
    if (!collider)
        return;
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return;
    UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);
    if (!tree)
        return;
    UnkShape* shape = collider->shape;
    if (!shape)
        return;

    u32 mask = flagged->mask;
    UnkSweepOffset_l7feb70 sweep(Offset_l7feb70(*pos, self->owner->Position()));
    UnkSweepQuery_l7feb70 query;
    const UnkMatrix44& xf = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->transform));
    if (SweepShapeAgainstCollisionTree(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf),
                      reinterpret_cast<const Vec2*>(&sweep.v), &query, false)) {
        f32 t = fn_7100977b20(flagged);
        pos->x = t * query.hit.normal.x + query.hit.point.x;
        pos->y = t * query.hit.normal.y + query.hit.point.y;
        pos->z = t * query.hit.normal.z + query.hit.point.z;
        pos->z = self->owner->Position().z;
    }
    if (IsNear_l7feb70(self->owner->transform[0][3], pos->x) && IsNear_l7feb70(self->owner->transform[1][3], pos->y) &&
        IsNear_l7feb70(self->owner->transform[2][3], pos->z))
        return;
    if (UnkAreaFXPart_l7feb70* part = self->owner->part)
        part->moved = 1;
    self->owner->vt->SetPosition(self->owner, pos, true);
}
