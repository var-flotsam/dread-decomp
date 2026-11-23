// CHyperBeamGun unit (tools/tu_map.py, unit 1305): the gun's update. Runs the base update, notes whether the
// owner's animation allows the charge, then either lets the glow fade while the gun is idle, or charges it
// (growing the glow part, and once full, sending the charged action), or reacts to the fire state changing.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CFXComponent.h"
#include "game/CHyperBeamGun.h"
#include "game/CInputComponent.h"
#include "game/CPrimaryGun.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::math::CVector3D;

// The animation name the charge is allowed under.
extern base::global::CStrId g_unk_7172726598 __attribute__((visibility("hidden")));

struct UnkHyperBeamGun_b56;

// The glow part: its slot 0x20 sets its scale.
struct UnkHyperBeamGlow_l8435f0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void SetScale(const CVector3D* scale);
};

struct UnkHyperBeamOwner_l8435f0 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x40];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x58];
    UnkWorld* fx;  // 0x280
};
static_assert(__builtin_offsetof(UnkHyperBeamOwner_l8435f0, fx) == 0x280, "layout");

struct UnkHyperBeamGun_l8435f0 {
    void* vtable;
    u8 pad_0x8[0x10];
    UnkHyperBeamOwner_l8435f0* owner;  // 0x18
    u8 pad_0x20[0x1880];
    u32 fire_state;  // 0x18a0
    u32 field_0x18a4;
    u8 pad_0x18a8[0x50];
    bool can_charge;  // 0x18f8
    u8 pad_0x18f9;
    bool active;  // 0x18fa
    u8 pad_0x18fb;
    f32 field_0x18fc;
    bool charged;  // 0x1900
    bool field_0x1901;
    bool field_0x1902;
    u8 pad_0x1903;
    f32 charge;  // 0x1904
    f32 charge_max;  // 0x1908
    f32 field_0x190c;
    f32 delay;  // 0x1910
    f32 glow;  // 0x1914
};
static_assert(__builtin_offsetof(UnkHyperBeamGun_l8435f0, glow) == 0x1914, "layout");

// The beam gun base's update (as src/game/CGrappleBeamGun/CGrappleBeamGun_Update_ovb0132.cpp declares it).
struct UnkGrappleBeamGun_ovb0132;
void fn_7100844408(UnkHyperBeamGun_l8435f0* self);

static const u64 kGlowPart = 0x805489787ebd4a19;

static inline UnkHyperBeamGlow_l8435f0* Glow(UnkHyperBeamGun_l8435f0* self) {
    return reinterpret_cast<UnkHyperBeamGlow_l8435f0*>(fn_7101152b90(self->owner->fx, kGlowPart));
}

static inline bool IsFireForced(UnkHyperBeamGun_l8435f0* self) {
    return fn_710117b348(self->owner->animation, fn_7100c1e458());
}

// Stops the gun unless the fire is forced.
static inline void Stop(UnkHyperBeamGun_l8435f0* self) {
    if (IsFireForced(self))
        return;
    fn_7100844408(self);
    self->active = false;
    self->fire_state = 0;
    self->field_0x18a4 = 0;
    self->field_0x18fc = 0.0f;
    self->field_0x190c = 0.0f;
    self->charge = 0.0f;
}

void fn_71008435f0(UnkHyperBeamGun_l8435f0* self, f32 dt) {
    fn_7100857418(reinterpret_cast<UnkGrappleBeamGun_ovb0132*>(self));
    UnkAnimationComponent* animation = self->owner->animation;
    self->can_charge =
        fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(animation),
                      reinterpret_cast<UnkCls_710117af10**>(&g_unk_7172726598), false) ||
        fn_710117ae38(self->owner->animation, 0x8b433509a5380892, false);

    if (!self->active) {
        self->field_0x18fc += dt;
        UnkHyperBeamGlow_l8435f0* glow = Glow(self);
        if (glow && self->glow > 0.0f) {
            self->glow -= dt;
            f32 s = self->glow / self->charge_max * 0.5f;
            CVector3D scale = {s, s, s};
            glow->SetScale(&scale);
            if (self->glow <= 0.0f)
                fn_7101152c0c(reinterpret_cast<UnkObject*>(self->owner->fx), glow, 0, 1);
        }
        return;
    }

    if (fn_710087238c(self->owner->input) && !IsFireForced(self)) {
        Stop(self);
        return;
    }

    u32 previous = self->fire_state;
    u32 current = fn_71008434b8(self);
    self->fire_state = current;
    if (previous != 0) {
        if (current == 0) {
            fn_7100843a50(reinterpret_cast<char*>(self), false);
            return;
        }
        self->field_0x18fc += dt;
        return;
    }
    if (current != 0) {
        fn_7100844408(self);
        return;
    }
    if (self->charged && self->field_0x1902)
        return;

    // The glow part's scale while it charges.
    CVector3D scale;
    if (self->delay > 0.0f)
        self->delay -= dt;
    else
        self->charge += dt;
    if (self->charge > 0.0f) {
        if (UnkHyperBeamGlow_l8435f0* glow = Glow(self)) {
            f32 s = self->charge / self->charge_max * 0.5f;
            scale = {s, s, s};
            glow->SetScale(&scale);
        }
    }

    if (!(self->charged && self->field_0x1902) && self->charge >= self->charge_max) {
        self->charge = self->charge_max;
        self->charged = true;
        self->field_0x1902 = true;
        if (self->can_charge) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
            fn_7101176690(animation, 0x33963557fd146e7a, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
        fn_71008449ec(reinterpret_cast<UnkHyperBeamGun_b56*>(self), true, !self->field_0x1901);
        self->field_0x1901 = true;
        self->glow = 0.0f;
    } else {
        self->glow = self->charge;
    }
}
