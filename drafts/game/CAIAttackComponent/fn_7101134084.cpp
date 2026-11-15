// CAIAttackComponent unit: runs the base update, then while the owner's frostbite component is not frozen
// forwards the time step to the attack object's virtual slot 0x40. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/CFrozenComponent.h"
#include "types.h"

struct UnkAttackInner_l134084 {
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

struct UnkAttackObject_l134084 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(f32 dt);
    u8 pad_0x8[0x18];
    UnkAttackInner_l134084* inner;
};

struct UnkOwner_l134084 {
    u8 pad_0x0[0x2c8];
    void* frostbite;
};

struct UnkAttackAI_l134084 {
    u8 pad_0x0[0x30];
    UnkOwner_l134084* owner;
    u8 pad_0x38[0x168];
    UnkAttackObject_l134084* attack;
};

static inline bool FrostbiteActive(UnkAttackAI_l134084* self) {
    void* frostbite = self->owner->frostbite;
    bool active = self->attack->inner->vf_0x48();
    return frostbite && active && fn_71007f9f20(frostbite);
}

static inline bool IsFrozen(UnkAttackAI_l134084* self) {
    return self->attack && FrostbiteActive(self);
}

void fn_7101134084(UnkAttackAI_l134084* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (!IsFrozen(self) && self->attack)
        self->attack->vf_0x40(dt);
}
