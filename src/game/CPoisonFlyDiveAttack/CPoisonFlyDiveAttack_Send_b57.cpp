// The CPoisonFlyDiveAttack unit. fn_7100632ee4: when `actor` is the one the owner (+0x8) acts against
// (fn_7100d7c990) and the state at +0x54 is 4, it sets the flag at +0x58, sends the owner's sub-object
// at +0x220 hash 0x81795dc06e33e40a under the usual track kind with mode 0 (UnkSpitSend,
// src/game_s2/UnkSpitSend.h), and moves to state 5 around two calls of fn_7100631c90. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "types.h"

void fn_7100631c90(void* self);

struct UnkPoisonFlyDive_b57a4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x44];
    u32 field_0x54;
    bool field_0x58;
};

void fn_7100632ee4(UnkPoisonFlyDive_b57a4* self, void* unused, UnkActor* actor) {
    if (!actor)
        return;
    void* target = self->owner->field_0x220;
    if (fn_7100d7c990(self->owner) != actor || self->field_0x54 != 4)
        return;
    self->field_0x58 = true;
    UnkSpitSend(target, 0x81795dc06e33e40a, 0xf4061ff25a2163d6);
    fn_7100631c90(self);
    self->field_0x54 = 5;
    fn_7100631c90(self);
}

