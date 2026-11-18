// CGunComponent unit (tools/tu_map.py unit 1304): a weapon was made current. A grapple beam ends the
// pending grapple (+0x130, the tunable at +0x98 back to 1); the weapon's vf_0x120 runs; the gun mode is
// brought up to date (as CGunComponent_Charge_ovb0091.cpp), and with `notify` the weapon's id is handed to
// the game manager's +0x1ba8 object. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

// A weapon of one of the gun's slots.
struct UnkGunWeapon_l83eeec {
    virtual const base::reflection::CType* GetType();  // 0x0
    virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50();
    virtual void vf_0x58(); virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78();
    virtual void vf_0x80(); virtual void vf_0x88(); virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0();
    virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8(); virtual void vf_0xc0(); virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void FireCharged(s32 mode, bool flag);  // 0xd8
    virtual void vf_0xe0(); virtual void vf_0xe8(); virtual void vf_0xf0(); virtual void vf_0xf8(); virtual void vf_0x100();
    virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120();

    u8 pad_0x8[0x18];
    CStrId id;  // 0x20
};

struct UnkGunTunable_l83eeec {
    u8 pad_0x0[0x98];
    f32 field_0x98;  // 0x98
};

struct UnkGunModeSource_l83eeec {
    u8 unk_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkGunModeState_l83eeec {
    u8 unk_0x0[0x20];
    UnkGunModeSource_l83eeec* source;  // 0x20
    u8 unk_0x28[0x8c];
    s32 kind;  // 0xb4
};

struct UnkGun_l83eeec {
    u8 pad_0x0[0x118];
    UnkGunWeapon_l83eeec* alt;   // 0x118
    UnkGunWeapon_l83eeec* main;  // 0x120
    bool use_alt;                // 0x128
    u8 pad_0x129[0x7];
    u8 field_0x130;
    u8 field_0x131;
    u8 pad_0x132[0x2];
    s32 mode;  // 0x134
    u8 pad_0x138[0x38];
    f32 charge_start;  // 0x170
    f32 charge_full;   // 0x174
    bool charging;     // 0x178
    u8 pad_0x179[0x3];
    f32 charge;        // 0x17c
    u8 field_0x180;
    u8 field_0x181;
    u8 pad_0x182[0x2];
    f32 field_0x184;
    u8 field_0x188;
    bool field_0x189;
};
static_assert(__builtin_offsetof(UnkGun_l83eeec, field_0x189) == 0x189, "layout");

// What the game manager's +0x1ba8 object is given: the weapon's id, a scale and a flag.
struct UnkGunPick_l83eeec {
    CStrId id;
    f32 scale;
    bool flag;
};

struct UnkGunPicks_l83eeec;
void fn_7100841f60(UnkGunPicks_l83eeec* self, const UnkGunPick_l83eeec* pick);

static inline UnkGunTunable_l83eeec* GunTunable() {
    return reinterpret_cast<UnkGunTunable_l83eeec*>(&g_unk_7172790a08);
}

// The weapon when it is a CGrappleBeamGun, null otherwise.
static inline UnkGunWeapon_l83eeec* AsGrappleBeam(UnkGunWeapon_l83eeec* weapon) {
    return fn_710009d50c(weapon->GetType(), fn_7100832034()) ? weapon : nullptr;
}

static inline UnkGunWeapon_l83eeec* ActiveWeapon(UnkGun_l83eeec* self) {
    return self->use_alt ? self->alt : self->main;
}

// Mode 2 when there is an active weapon and the game manager's gun state is of kind 6 (for kind 7,
// its object at +0x20 decides), mode 1 otherwise.
static inline s32 GunMode(UnkGun_l83eeec* self) {
    if (!ActiveWeapon(self))
        return 1;
    auto* state = static_cast<UnkGunModeState_l83eeec*>(fn_7100df5460(g_unk_7101d47310));
    if (!state)
        return 1;
    s32 kind = state->kind;
    if (kind == 7) {
        if (!state->source)
            return 1;
        kind = state->source->state;
    }
    return kind == 6 ? 2 : 1;
}

// Ends the charge: fires the active weapon's charged shot first when `fire` is set.
static inline void Release(UnkGun_l83eeec* self, bool fire) {
    if (fire) {
        UnkGunWeapon_l83eeec* weapon = ActiveWeapon(self);
        if (weapon)
            weapon->FireCharged(self->mode, false);
    }
    self->charging = false;
    self->charge = 0.0f;
    self->field_0x180 = 0;
    self->field_0x181 = 0;
    self->field_0x184 = -1.0f;
    self->field_0x189 = false;
}

static inline void CancelCharge(UnkGun_l83eeec* self) {
    if (self->field_0x181) {
        Release(self, true);
    } else if (self->charging) {
        self->field_0x131 = 0;
        if (!self->field_0x188)
            Release(self, self->charge >= self->charge_start);
    }
}

static inline void UpdateMode(UnkGun_l83eeec* self) {
    s32 mode = GunMode(self);
    if (self->mode != mode) {
        self->mode = mode;
        CancelCharge(self);
    }
}

void fn_710083eeec(UnkGun_l83eeec* self, UnkGunWeapon_l83eeec* weapon, void* unused, bool notify) {
    // Equipping the grapple beam ends the pending grapple.
    if (weapon && self->field_0x130 && AsGrappleBeam(weapon)) {
        self->field_0x130 = 0;
        GunTunable()->field_0x98 = 1.0f;
    }
    if (weapon)
        weapon->vf_0x120();
    UpdateMode(self);
    if (notify) {
        CStrId id = weapon->id;
        UnkGunPick_l83eeec pick = {id, 1.0f, false};
        fn_7100841f60(reinterpret_cast<UnkGunPicks_l83eeec*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x1ba8),
                      &pick);
    }
}
