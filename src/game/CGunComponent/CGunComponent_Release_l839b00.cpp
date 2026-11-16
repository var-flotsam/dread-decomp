// CGunComponent (tools/tu_map.py unit 1304): ends the gun's charge. A full charge fires the active weapon
// (its vf_0xd8) unless the grab state or a cancelling animation track holds it; the charge state is then
// cleared. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CGunComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkGunWeapon_l839b00 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8(s32 mode, bool cancelled);
};

struct UnkGunActor_l839b00 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGunGrab_l839b00 {
    u8 pad_0x0[0x18];
    bool field_0x18;  // 0x18
};

struct UnkGunComponent_l839b00 {
    u8 pad_0x0[0x30];
    UnkGunActor_l839b00* actor;  // 0x30
    u8 pad_0x38[0xe0];
    UnkGunWeapon_l839b00* alt;   // 0x118
    UnkGunWeapon_l839b00* main;  // 0x120
    bool use_alt;                // 0x128
    u8 pad_0x129[0x8];
    bool field_0x131;  // 0x131
    u8 pad_0x132[0x2];
    s32 mode;  // 0x134
    u8 pad_0x138[0x38];
    f32 charge_full;  // 0x170
    u8 pad_0x174[0x4];
    bool charging;  // 0x178
    u8 pad_0x179[0x3];
    f32 charge;          // 0x17c
    bool field_0x180;    // 0x180
    bool field_0x181;    // 0x181
    u8 pad_0x182[0x2];
    f32 field_0x184;     // 0x184
    bool field_0x188;        // 0x188
    bool field_0x189;    // 0x189
    u8 pad_0x18a[0x13e];
    UnkGunGrab_l839b00* grab;  // 0x2c8

    UnkGunWeapon_l839b00* Weapon() const { return use_alt ? alt : main; }
};
static_assert(__builtin_offsetof(UnkGunComponent_l839b00, grab) == 0x2c8, "UnkGunComponent_l839b00 layout");

void fn_710083d27c(UnkSamusGunComponent* gun, bool flag);

void fn_7100839b00(char* self_, bool value) {
    auto* self = reinterpret_cast<UnkGunComponent_l839b00*>(self_);
    if (self->field_0x181) {
        if (UnkGunWeapon_l839b00* weapon = self->Weapon())
            weapon->vf_0xd8(self->mode, false);
    } else {
        if (!self->charging)
            return;
        self->field_0x131 = false;
        bool cancelled = false;
        if (value && self->mode == 1 && self->charge >= self->charge_full) {
            if (!self->grab || self->grab->field_0x18) {
                fn_710083d27c(reinterpret_cast<UnkSamusGunComponent*>(self), self->use_alt);
                self->field_0x188 = self->grab ? self->grab->field_0x18 : false;
            } else {
                self->field_0x131 = true;
                if (UnkGunActor_l839b00* actor = self->actor) {
                    if (UnkAnimationComponent* animation = actor->animation) {
                        bool grabbing = fn_710117b348(animation, fn_7100cff148()) ||
                                        fn_710117b348(animation, fn_7100cfe868());
                        bool dissipating = fn_710117b348(animation, fn_7100c14f2c());
                        cancelled = grabbing || dissipating;
                        self->field_0x131 = !cancelled;
                    }
                }
            }
            if (self->field_0x131)
                return;
        }
        if (self->field_0x188)
            return;
        if (self->charge >= self->charge_full) {
            if (UnkGunWeapon_l839b00* weapon = self->Weapon())
                weapon->vf_0xd8(self->mode, cancelled);
        }
    }
    self->charging = false;
    self->charge = 0.0f;
    self->field_0x180 = false;
    self->field_0x181 = false;
    self->field_0x184 = -1.0f;
    self->field_0x189 = false;
}
