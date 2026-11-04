// A routine of the CHydrogigaTentacleBashAttack unit: after fn_71005b2c58, sends one of two fixed
// requests with mode 1 to the owner's sub-object (+0x220 of the owner at +0x8), picked by whether the
// owner's target is left of the owner; descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). Names
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "types.h"

void fn_71005b2c58(void* self);

struct UnkTentacleBashAttack_b57a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_71005b6f9c(UnkTentacleBashAttack_b57a3* self) {
    fn_71005b2c58(self);
    void* target = self->owner->field_0x220;
    UnkActor* other = fn_7100d7c990(self->owner);
    if (!target || !other)
        return;
    u64 hash = !(other->field_0x44 >= self->owner->field_0x44) ? 0x410ba119e1f0ca4b : 0xbbd901f21b4f3973;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 1;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
