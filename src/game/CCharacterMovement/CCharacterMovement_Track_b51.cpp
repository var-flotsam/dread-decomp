// CCharacterMovement (unit 1445): the flag at +0x3aa that says the owner is held by a track or
// state (see fn_710117b348). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"

struct UnkMovement_71010d0e38;

struct UnkHoldState_1445 {
    u8 pad_0x0[0x70];
    u32 kind;
};

struct UnkOwner_1445 {
    u8 pad_0x0[0x1c8];
    UnkHoldState_1445* hold;
    u8 pad_0x1d0[0x50];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMovement_1445 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_1445* owner;  // 0x30
    u8 pad_0x38[0xc1];
    bool field_0xf9;
    u8 pad_0xfa[0x3a8 - 0xfa];
    bool field_0x3a8;
    u8 pad_0x3a9;
    bool field_0x3aa;

    void vf_0x260() { reinterpret_cast<void (*)(UnkMovement_1445*)>(vtable[0x260 / 8])(this); }
    bool vf_0x2c8() { return reinterpret_cast<bool (*)(UnkMovement_1445*)>(vtable[0x2c8 / 8])(this); }
    void vf_0x320() { reinterpret_cast<void (*)(UnkMovement_1445*)>(vtable[0x320 / 8])(this); }
};

void fn_7100978364(UnkMovement_1445* self) {
    if (!self->field_0x3a8) {
        self->field_0x3aa = false;
        return;
    }
    if (self->field_0xf9) {
        self->field_0xf9 = false;
        self->vf_0x260();
    }
    if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)))
        return;
    UnkOwner_1445* owner = self->owner;
    if (owner->hold && owner->hold->kind == 2) {
        self->field_0x3aa = true;
        return;
    }
    if (UnkAnimationComponent* animation = owner->animation) {
        bool held = fn_710117b348(animation, fn_7100d09928());
        self->field_0x3aa = held;
        if (held)
            return;
    } else {
        self->field_0x3aa = false;
    }
    if (self->vf_0x2c8())
        self->vf_0x320();
}
