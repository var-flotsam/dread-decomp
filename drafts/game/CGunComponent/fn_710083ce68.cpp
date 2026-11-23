// CGunComponent unit (tools/tu_map.py unit 1304): decides, from the grapple selection mode of the grapple
// beam's tunable, whether the gun's alternative weapon switches to the grapple beam or back from it.
// Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CGrabComponent.h"
#include "game/CGrappleBeamGun.h"
#include "game/CGunComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

bool fn_7100df5b00(UnkGameManager* gm);

// The grapple selection mode of the grapple beam's tunable.
struct UnkGrappleTunable_l83ce68 {
    u8 pad_0x0[0x2c];
    u32 mode;  // 0x2c
};

struct UnkState_l83ce68 {
    u8 pad_0x0[0x10];
    s32 value;  // 0x10
};

struct UnkStateHolder_l83ce68 {
    u8 pad_0x0[0x50];
    UnkState_l83ce68* state;  // 0x50
};

struct UnkGunActor_l83ce68 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    void* grab;  // 0x260
    u8 pad_0x268[0x48];
    UnkStateHolder_l83ce68* holder;  // 0x2b0
};

struct UnkGrappleTarget_l83ce68 {
    u8 pad_0x0[0x490];
    UnkState_l83ce68* state;  // 0x490
};

struct UnkGrappleSlot_l83ce68 {
    u8 pad_0x0[0x10];
    UnkGrappleTarget_l83ce68* target;  // 0x10
};

struct UnkGrappleGun_l83ce68 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual bool IsEnabled();  // 0x30
    u8 pad_0x8[0x1869];
    bool ready;  // 0x1871
    u8 pad_0x1872[0x16];
    UnkGrappleSlot_l83ce68* slot;  // 0x1888
};
static_assert(__builtin_offsetof(UnkGrappleGun_l83ce68, slot) == 0x1888, "layout");

struct UnkGunWeapon_l83ce68 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88();
    virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8();
    virtual void vf_0xf0(); virtual void vf_0xf8(); virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130(); virtual void vf_0x138();
    virtual bool IsReady();  // 0x140
    u8 pad_0x8[0x18];
    CStrId id;  // 0x20
};

struct UnkGunComponent_l83ce68 {
    u8 pad_0x0[0x30];
    UnkGunActor_l83ce68* actor;  // 0x30
    u8 pad_0x38[0xe0];
    UnkGunWeapon_l83ce68* alt;  // 0x118
    u8 pad_0x120[0x18];
    u32 state;  // 0x138
    u8 pad_0x13c[0x4];
    base::UnkStrIdVec previous;  // 0x140
    CStrId id;  // 0x168
    u8 pad_0x170[0xc];
    f32 field_0x17c;
};
static_assert(__builtin_offsetof(UnkGunComponent_l83ce68, id) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l83ce68, field_0x17c) == 0x17c, "layout");

// The grapple beam's name.
static inline const CStrId& GrappleId_l83ce68() {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x248];
}

// Makes the grapple beam the alternative weapon.
static inline void SelectGrapple_l83ce68(UnkGunComponent_l83ce68* self, const CStrId& grapple,
                                         UnkGunWeapon_l83ce68* alt) {
    if (!alt || alt->id == grapple)
        return;
    self->state = 2;
    if (self->field_0x17c < 0.01f && alt->IsReady()) {
        fn_710083f32c(reinterpret_cast<UnkSamusGunComponent*>(self), &grapple, true, true, false);
        return;
    }
    self->id = grapple;
}

// Switches back from the grapple beam.
static inline void RestoreGrapple_l83ce68(UnkGunComponent_l83ce68* self, const CStrId& grapple,
                                          UnkGunWeapon_l83ce68* alt) {
    if (!alt)
        return;
    if (!(self->id == grapple)) {
        if (self->previous.count != 0 && alt->id == grapple && alt->IsReady())
            fn_710083ad6c(reinterpret_cast<UnkSamusGunComponent*>(self));
        return;
    }
    self->id = g_unk_7101d470e0;
}

void fn_710083ce68(UnkGunComponent_l83ce68* self, bool select) {
    if (fn_7100df5b00(g_unk_7101d47310))
        return;
    if (UnkAnimationComponent* animation = self->actor->animation)
        if (fn_710117b348(animation, fn_7100c2eae8()))
            return;

    UnkGrappleGun_l83ce68* gun = reinterpret_cast<UnkGrappleGun_l83ce68*>(
        fn_710030c5fc(reinterpret_cast<UnkAuraGunOwner*>(self), GrappleId_l83ce68()));
    if (!gun || !gun->IsEnabled())
        return;
    if (gun->ready && self->previous.count != 0 && self->state != 2)
        return;

    if (self->actor->grab)
        select = !fn_710113d83c(self->actor->grab) && select;
    bool held = false;
    if (UnkStateHolder_l83ce68* holder = self->actor->holder)
        if (UnkState_l83ce68* state = holder->state)
            held = state->value == 2;

    switch (reinterpret_cast<UnkGrappleTunable_l83ce68*>(&g_unk_7172725bc8)->mode) {
    case 1:
        if (!gun->ready || !select)
            return;
        RestoreGrapple_l83ce68(self, GrappleId_l83ce68(), self->alt);
        return;
    case 2:
        if (select && held) {
            SelectGrapple_l83ce68(self, GrappleId_l83ce68(), self->alt);
            return;
        }
        if (!gun->ready)
            return;
        if (gun->slot && gun->slot->target) {
            UnkState_l83ce68* state = gun->slot->target->state;
            if (!state || state->value != 1)
                return;
        }
        RestoreGrapple_l83ce68(self, GrappleId_l83ce68(), self->alt);
        return;
    case 3: {
        const CStrId& grapple = GrappleId_l83ce68();
        UnkGunWeapon_l83ce68* alt = self->alt;
        if (select)
            SelectGrapple_l83ce68(self, grapple, alt);
        else
            RestoreGrapple_l83ce68(self, grapple, alt);
        return;
    }
    }
}
