// Unit 2702 (hint CAIComponent): unless the owner's target is held (its tracked entry answers
// fn_7100979ef0, or its character class has a climbing track), fills `out` with the target's position
// and marks the first two fields all-ones. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU55.h"
#include "types.h"

struct UnkPos3_l100250 {
    u32 a;
    u32 b;
    u32 c;
};

struct UnkTarget_l100250 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
    u8 pad_0x68[0x198];
    void* field_0x200;
    u8 pad_0x208[0x18];
    UnkAnimationComponent* field_0x220;
};

struct UnkFillOut_l100250 {
    u16 field_0x0;
    u8 field_0x2;
    u8 pad_0x3;
    u8 field_0x4[1];
};

struct UnkFillSelf_l100250 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

void fn_7101100250(UnkFillSelf_l100250* self, UnkFillOut_l100250* out) {
    UnkActor* owner = self->owner;
    UnkTarget_l100250* target = owner ? reinterpret_cast<UnkTarget_l100250*>(fn_7100d7c990(owner)) : nullptr;
    if (target) {
        void* tracked = target->field_0x200;
        UnkAnimationComponent* character = target->field_0x220;
        bool held = tracked ? fn_7100979ef0(tracked) : false;
        if (character && !held)
            held |= fn_710117b348(character, fn_7100cf25d8());
        if (held)
            return;
    }
    u32 x = target->field_0x44;
    u32 y = target->field_0x54;
    u32 z = target->field_0x64;
    UnkPos3_l100250 pos = {x, y, z};
    fn_710007b444(out->field_0x4, reinterpret_cast<const UnkVec3*>(&pos));
    out->field_0x2 = 0xff;
    out->field_0x0 = 0xffff;
}
