// CAIAttackComponent unit: per-frame update. Runs the base update, counts down two timers and lets the owner
// react when the attack asks for it, then forwards the time step to the attack
// object unless the owner is frozen and the attack is finished; clears two state fields.
// Names and layouts are placeholders.
#include "game/CFrozenComponent.h"
#include "types.h"

struct UnkAttackInner_l133efc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual bool vf_0x40();
};

struct UnkAttackObject_l133efc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(f32 dt);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual bool vf_0x68();
    u8 pad_0x8[0x18];
    UnkAttackInner_l133efc* inner;
};

struct UnkOwner_l133efc {
    u8 pad_0x0[0x2c8];
    void* frostbite;
};

struct UnkAttackAI_l133efc {
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
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    u8 pad_0x8[0x28];
    UnkOwner_l133efc* owner;
    u8 pad_0x38[0x168];
    UnkAttackObject_l133efc* attack;
    u8 pad_0x1a8[4];
    f32 timer_a;
    f32 timer_b;
    u8 pad_0x1b4[0x114];
    u32 field_0x2c8;
    u8 pad_0x2cc[4];
    u8 field_0x2d0;
};

void fn_7101136310(UnkAttackAI_l133efc* self);

void fn_7101133efc(UnkAttackAI_l133efc* self, f32 dt) {
    fn_7101136310(self);
    if (self->timer_a > 0)
        self->timer_a -= dt;
    if (self->timer_b > 0)
        self->timer_b -= dt;
    if (self->attack && self->attack->vf_0x68())
        self->vf_0x178();
    void* frostbite = self->owner->frostbite;
    bool finished = frostbite && fn_71007f9f20(frostbite) && self->attack && !self->attack->inner->vf_0x40();
    if (!finished && self->attack)
        self->attack->vf_0x30(dt);
    self->field_0x2d0 = 0;
    self->field_0x2c8 = 0;
}
