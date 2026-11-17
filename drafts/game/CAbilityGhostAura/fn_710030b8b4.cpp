// CAbilityGhostAura unit: probes straight down from the given point with the owner's movement (slot 0x2a8 of
// the object at +0x200 of the owner's actor); true when the probe hits something whose record has a link at
// +0x10 and fn_71010d9638 does not accept the hit. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU2689.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCharacterMovement;
struct UnkCollisionHit;
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);

struct UnkHitObject_l30b8b4 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkHitRef_l30b8b4 {
    UnkHitObject_l30b8b4* ptr;

    UnkHitRef_l30b8b4() : ptr(nullptr) {}
    ~UnkHitRef_l30b8b4() {
        if (UnkHitObject_l30b8b4* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l30b8b4 {
    f32 x, y, z;
    UnkHitVec_l30b8b4(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l30b8b4 {
    UnkHitRef_l30b8b4 object;
    UnkHitRef_l30b8b4 field_0x8;
    s32 id;
    UnkHitVec_l30b8b4 point;
    UnkHitVec_l30b8b4 field_0x20;
    UnkHitVec_l30b8b4 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l30b8b4()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkMove_b152 {
    void** vtable;
    u8 pad_0x8[0x3c8];
    u32 collision_mask;  // 0x3d0

    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkMove_b152*)>(vtable[0x2a8 / 8])(this);
    }
};

struct UnkGhostAuraActor_l30b8b4 {
    u8 pad_0x0[0x200];
    UnkMove_b152* movement;
};

struct UnkGhostAuraOwner_l30b8b4 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_l30b8b4* actor;
};

struct UnkSelf {
    u8 pad_0x0[0xc0];
    UnkGhostAuraOwner_l30b8b4* owner;
};

bool fn_710030b8b4(UnkSelf* self, void* a2, void* zero) {
    UnkGhostAuraOwner_l30b8b4* owner = self->owner;
    UnkMove_b152* movement = owner && owner->actor ? owner->actor->movement : nullptr;
    UnkCharacterMovement* character = movement ? movement->vf_0x2a8() : nullptr;
    UnkCollisionHit_l30b8b4 hit;
    bool blocked = false;
    if (character) {
        bool found;
        {
            u32 mask = movement->collision_mask;
            UnkVec3f down(0.0f, -1.0f, 0.0f);
            found = fn_7100977e50(character, -0.3f, 20.0f, 3.4028235e38f, 0.0f, mask,
                                  static_cast<const CVector3D*>(a2), static_cast<CVector3D*>(zero),
                                  reinterpret_cast<UnkCollisionHit*>(&hit), reinterpret_cast<const CVector3D*>(&down));
        }
        if (found && hit.object.ptr && hit.object.ptr->field_0x10) {
            u64 out = 0;
            blocked = !fn_71010d9638(movement, reinterpret_cast<UnkHitInfo_b152*>(&hit), &out);
        }
    }
    return blocked;
}
