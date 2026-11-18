// CMorphBallMovement's unit (tools/tu_map.py unit 1428): the ground probe of the ball. In states 2, 4
// and 5 (and, for kind 2, unless the other object's mask (slot 0x58) meets the ball's mask +0x3d0)
// probes from the owner's position along the stored direction (+0x198 while the handle at +0x178 is
// alive, else down) with fn_7100977e50. A hit the base handler (fn_71010d9638) does not take is kept
// (slot 0x218) with +0x2540 telling whether it has no target; otherwise +0x2540 is set, the base
// contact (fn_710097a664) gets an empty hit and fn_710093ed64 runs. Names and layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2689.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

namespace base {
namespace math {
struct CVector3D;
}
}  // namespace base
using base::math::CVector3D;

struct UnkCharacterMovement;
struct UnkCollisionHit;
struct UnkMorphBall_eb64;

// A counted reference to a weak handle, released through the handle's virtual 0x10.
struct UnkHitRef_l94b548 {
    UnkWeakHandle* ptr;

    UnkHitRef_l94b548() : ptr(nullptr) {}
    ~UnkHitRef_l94b548() {
        if (UnkWeakHandle* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The collision hit fn_7100977e50 fills in (0x48 bytes).
struct UnkProbeHit_l94b548 {
    UnkHitRef_l94b548 object;
    UnkHitRef_l94b548 field_0x8;
    s32 id;
    f32 values[12];

    UnkProbeHit_l94b548() : id(-1), values() {}
};
static_assert(sizeof(UnkProbeHit_l94b548) == 0x48, "layout");

struct UnkMorphBallTuning_l94b548 {
    u8 pad_0x0[0x98];
    f32 max_dot;  // 0x98
};

struct UnkOther_l94b548 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
};

struct UnkMorphBallActor_l94b548 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkMorphBall_l94b548 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkMorphBallActor_l94b548* owner;  // 0x30
    u8 pad_0x38[0x178 - 0x38];
    UnkWeakHandle* field_0x178;
    u8 pad_0x180[0x18];
    UnkVec3f field_0x198;
    u8 pad_0x1a4[0x3d0 - 0x1a4];
    u32 mask;  // 0x3d0
    u8 pad_0x3d4[0x2280 - 0x3d4];
    UnkMorphBallTuning_l94b548* tuning;  // 0x2280
    u32 state;                           // 0x2288
    u8 pad_0x228c[0x2438 - 0x228c];
    s32 field_0x2438;
    u8 pad_0x243c[0x2540 - 0x243c];
    bool field_0x2540;

    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkMorphBall_l94b548*)>(vtable[0x2a8 / 8])(this);
    }
    void vf_0x218(UnkProbeHit_l94b548* hit) {
        reinterpret_cast<void (*)(UnkMorphBall_l94b548*, UnkProbeHit_l94b548*)>(vtable[0x218 / 8])(this, hit);
    }
};

void fn_710097a664(UnkMorphBall_l94b548* self, UnkProbeHit_l94b548* hit);

void fn_710094b548(UnkMorphBall_l94b548* self, UnkOther_l94b548* other, s32 kind) {
    u32 state = self->state;
    if (state != 2 && state != 4 && state != 5)
        return;
    if (kind == 2 && (self->mask & other->vf_0x58()))
        return;

    UnkProbeHit_l94b548 hit;
    bool alive = self->field_0x178 && self->field_0x178->target;
    UnkVec3f dir = alive ? self->field_0x198 : UnkVec3f(0.0f, -1.0f, 0.0f);
    bool found;
    {
        UnkCharacterMovement* movement = self->vf_0x2a8();
        UnkVec3f pos;
        UnkMorphBallActor_l94b548* owner = self->owner;
        u32 mask = self->mask;
        pos = UnkVec3f(owner->x, owner->y, owner->z);
        found = fn_7100977e50(movement, self->tuning->max_dot, 5.0f, 3.4028235e38f, 40.0f, mask,
                              reinterpret_cast<const CVector3D*>(&pos), nullptr,
                              reinterpret_cast<UnkCollisionHit*>(&hit), reinterpret_cast<const CVector3D*>(&dir));
    }
    if (found) {
        u64 result = 0;
        if (!fn_71010d9638(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<UnkHitInfo_b152*>(&hit),
                           &result)) {
            self->field_0x2540 = !(hit.object.ptr && hit.object.ptr->target);
            self->field_0x2438 = -1;
            self->vf_0x218(&hit);
            return;
        }
    }
    self->field_0x2540 = true;
    {
        UnkProbeHit_l94b548 none;
        fn_710097a664(self, &none);
    }
    fn_710093ed64(reinterpret_cast<UnkMorphBall_eb64*>(self));
}
