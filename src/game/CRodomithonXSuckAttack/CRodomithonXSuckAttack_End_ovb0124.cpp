// CRodomithonXSuckAttack's unit (tools/tu_map.py unit 986): the attack's end. After the shared attack reset
// (fn_71003609b8) and the AI component's own (fn_71010fa790), unless the owner's frozen component (+0x2c8,
// fn_71007f9f20) holds, the owner's +0x220 is sent a zero under a fixed hash (refl_EmitU32). Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAttack.h"
#include "types.h"

void fn_71010fa790(void* ai);

struct UnkSuckOwner_ovb0124 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    void* field_0x240;
    u8 pad_0x248[0x80];
    void* field_0x2c8;
};

struct UnkSuckAttack_ovb0124 {
    void** vtable;
    UnkSuckOwner_ovb0124* owner;  // 0x8
};

void fn_7100664aa8(UnkSuckAttack_ovb0124* self) {
    void* target = self->owner->field_0x220;
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    fn_71010fa790(self->owner->field_0x240);
    void* frozen = self->owner->field_0x2c8;
    if (frozen && fn_71007f9f20(frozen))
        return;
    base::reflection::refl_EmitU32(target, 0x4f36c2305fee37d4, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                   &g_unk_7172769740);
}
