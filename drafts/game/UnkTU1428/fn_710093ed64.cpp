// CMorphBallMovement's unit: probes for the ground under the ball (fn_710093ed64): unless the game
// manager says so, casts fn_7100977e50 from the owner's position along the contact direction (or down),
// lets fn_71010d9638 veto the hit, keeps the grounded flag at +0x3a8 in step (vf_0x2b8 on a change) and
// hands the hit (or an empty one) to vf_0x218. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2689.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

struct UnkCharacterMovement;
struct UnkCollisionHit;
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);

struct UnkMorphBall_eb64;

// A counted reference released through the object's virtual 0x10.
struct UnkGroundRef_ed64 {
    UnkRefObject* ptr;

    UnkGroundRef_ed64() : ptr(nullptr) {}
    ~UnkGroundRef_ed64() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The collision hit fn_7100977e50 fills in (0x48 bytes).
struct UnkGroundHit_ed64 {
    UnkGroundRef_ed64 object;
    UnkGroundRef_ed64 field_0x8;
    s32 id;
    f32 values[12];

    UnkGroundHit_ed64() : id(-1), values() {}
};
static_assert(sizeof(UnkGroundHit_ed64) == 0x48, "UnkGroundHit_ed64 layout");

struct UnkGroundOwner_ed64 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkGroundLink_ed64 {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkMorphBallGround_ed64 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGroundOwner_ed64* owner;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    bool locked;  // 0xf9
    u8 pad_0xfa[0x128 - 0xfa];
    UnkGroundLink_ed64* contact;  // 0x128
    u8 pad_0x130[0x18];
    UnkVec3f contact_dir;  // 0x148
    u8 pad_0x154[0x3a8 - 0x154];
    bool grounded;  // 0x3a8
    u8 pad_0x3a9[0x27];
    u32 mask;  // 0x3d0
    u8 pad_0x3d4[0x2288 - 0x3d4];
    u32 state;  // 0x2288
    u8 pad_0x228c[0x2428 - 0x228c];
    u64 field_0x2428;
    u64 field_0x2430;
    s32 field_0x2438;

    void vf_0x218(UnkGroundHit_ed64* hit) {
        reinterpret_cast<void (*)(UnkMorphBallGround_ed64*, UnkGroundHit_ed64*)>(vtable[0x218 / 8])(this, hit);
    }
    f32 vf_0x290() { return reinterpret_cast<f32 (*)(UnkMorphBallGround_ed64*)>(vtable[0x290 / 8])(this); }
    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkMorphBallGround_ed64*)>(vtable[0x2a8 / 8])(this);
    }
    void vf_0x2b8() { reinterpret_cast<void (*)(UnkMorphBallGround_ed64*)>(vtable[0x2b8 / 8])(this); }

    // Whether fn_71010d9638 turns the hit down.
    bool RejectsHit(UnkGroundHit_ed64* hit) {
        u64 info = 0;
        return fn_71010d9638(reinterpret_cast<UnkMove_b152*>(this), reinterpret_cast<UnkHitInfo_b152*>(hit), &info);
    }

    void SetGrounded(bool value) {
        if (grounded != value) {
            grounded = value;
            vf_0x2b8();
        }
    }
};
static_assert(__builtin_offsetof(UnkMorphBallGround_ed64, field_0x2438) == 0x2438, "UnkMorphBallGround_ed64 layout");

static inline UnkVec3f OwnerPosition(const UnkGroundOwner_ed64* owner) {
    return UnkVec3f(owner->x, owner->y, owner->z);
}

void fn_710093ed64(UnkMorphBall_eb64* ball) {
    UnkMorphBallGround_ed64* self = reinterpret_cast<UnkMorphBallGround_ed64*>(ball);
    if (fn_7100deac04(g_unk_7101d47310))
        return;

    UnkGroundHit_ed64 hit;
    bool found;
    {
        UnkCharacterMovement* movement = self->vf_0x2a8();
        u32 mask = self->mask;
        UnkVec3f pos = OwnerPosition(self->owner);
        u32 state = self->state;
        UnkVec3f dir = ((state == 2 || state == 4 || state == 5) && self->contact && self->contact->target)
                           ? self->contact_dir
                           : UnkVec3f(0.0f, -1.0f, 0.0f);
        found = fn_7100977e50(movement, self->vf_0x290(), 20.0f, 3.4028235e38f, 0.0f, mask,
                              reinterpret_cast<const CVector3D*>(&pos), nullptr,
                              reinterpret_cast<UnkCollisionHit*>(&hit), reinterpret_cast<const CVector3D*>(&dir));
    }

    if (self->RejectsHit(&hit)) {
        self->SetGrounded(false);
        UnkGroundHit_ed64 none;
        self->vf_0x218(&none);
    } else {
        if (!self->locked)
            self->SetGrounded(found);
        self->vf_0x218(&hit);
    }
    self->field_0x2430 = 0;
    self->field_0x2428 = 0;
    self->field_0x2438 = -1;
}
