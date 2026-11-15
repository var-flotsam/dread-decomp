// CAIAttackComponent unit: while the owner's frostbite component is not frozen, forwards to the attack
// object's virtual slot 0x38. Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "types.h"

struct UnkAttackInner_l134008 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();
};

struct UnkAttackObject_l134008 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    u8 pad_0x8[0x18];
    UnkAttackInner_l134008* inner;
};

struct UnkOwner_l134008 {
    u8 pad_0x0[0x2c8];
    void* frostbite;
};

struct UnkAttackAI_l134008 {
    u8 pad_0x0[0x30];
    UnkOwner_l134008* owner;
    u8 pad_0x38[0x168];
    UnkAttackObject_l134008* attack;
};

static inline bool IsFrozen(UnkAttackAI_l134008* self) {
    UnkAttackObject_l134008* attack = self->attack;
    if (!attack)
        return false;
    void* frostbite = self->owner->frostbite;
    bool active = attack->inner->vf_0x48();
    return frostbite && active && fn_71007f9f20(frostbite);
}

void fn_7101134008(UnkAttackAI_l134008* self) {
    if (!IsFrozen(self) && self->attack)
        self->attack->vf_0x38();
}
