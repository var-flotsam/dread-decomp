// CQuetzoaAIComponent's unit (tools/tu_map.py unit 973): whether the owner's collision shape, placed at the
// owner's position and swept by the projection of `delta`, hits anything in the game manager's collision
// tracker or in the tracker of the owner's +0x240 object. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAbilityShinespark.h"
#include "game/CCharacterMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkMatrix44_7100078c9c.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "game/logic/collision/UnkBvhTreeSweep.h"
#include "types.h"

struct UnkQuetzoaPart_l64b884;
UnkTree* fn_710110c498(UnkQuetzoaPart_l64b884* part);

struct UnkHitObj_l64b884 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l64b884 {
    UnkHitObj_l64b884* ptr;

    UnkHitRef_l64b884() : ptr(nullptr) {}
    ~UnkHitRef_l64b884() {
        if (UnkHitObj_l64b884* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l64b884 {
    f32 x, y, z;
    UnkHitVec_l64b884(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l64b884 {
    UnkHitRef_l64b884 object;
    UnkHitRef_l64b884 field_0x8;
    s32 id;
    UnkHitVec_l64b884 point;
    UnkHitVec_l64b884 field_0x20;
    UnkHitVec_l64b884 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l64b884()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query the sweep fills.
struct UnkSweepQuery_l64b884 {
    void** vtable;
    UnkCollisionHit_l64b884 hit;
    bool field_0x50;

    UnkSweepQuery_l64b884() {
        field_0x50 = false;
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkSweepQuery_l64b884() { vtable = g_unk_7101a20ed8 + 2; }
};

// What fn_7100976588 returns: the collider whose shape is at +0x158.
struct UnkCollider_l64b884 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

struct UnkFlaggedMask_l64b884 {
    u8 pad_0x0[0x3cc];
    u32 mask;
};

struct UnkQuetzoaOwner_l64b884 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x1b0];
    UnkFlaggedMask_l64b884* flagged;  // 0x218
    u8 pad_0x220[0x20];
    UnkQuetzoaPart_l64b884* part;  // 0x240

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};
static_assert(__builtin_offsetof(UnkQuetzoaOwner_l64b884, part) == 0x240, "layout");

struct UnkQuetzoaAI_l64b884 {
    u8 pad_0x0[0x30];
    UnkQuetzoaOwner_l64b884* owner;
};

// A sweep offset: the x and y of a 3D one.
struct UnkSweepOffset_l64b884 {
    base::math::CVector2D v;

    explicit UnkSweepOffset_l64b884(const base::math::CVector3D* offset) { fn_710007b430(&v, offset); }
};

// Whether the shape placed by `xf` and swept by `sweep` hits anything in `tree` on the layers of `mask`.
static inline bool Sweep(UnkTree* tree, u32 mask, UnkShape* shape, const UnkMatrix44_7100078c9c& xf, const UnkSweepOffset_l64b884& sweep,
                         UnkSweepQuery_l64b884* query) {
    return SweepShapeAgainstCollisionTree(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(&sweep.v), query, false);
}

bool CQuetzoaAIComponent_SweepCollidesAlongDelta(UnkQuetzoaAI_l64b884* self, const base::math::CVector3D* delta) {
    UnkQuetzoaPart_l64b884* part = self->owner->part;
    UnkFlaggedMask_l64b884* flagged = self->owner->flagged;
    UnkCollider_l64b884* collider =
        flagged ? reinterpret_cast<UnkCollider_l64b884*>(fn_7100976588(reinterpret_cast<UnkFlagged*>(flagged)))
                : nullptr;
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);
    if (!part || !collider || !tree)
        return false;

    u32 mask = self->owner->flagged->mask;
    UnkSweepQuery_l64b884 query;
    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(self->owner->Position());
    bool hit = Sweep(tree, mask, collider->shape, xf, UnkSweepOffset_l64b884(delta), &query);
    if (fn_710110c498(part))
        hit |= Sweep(fn_710110c498(part), mask, collider->shape, xf, UnkSweepOffset_l64b884(delta), &query);
    return hit;
}
