// CGooplotJumpAttack unit (tools/tu_map.py, the name from reflection): when the owner's (+8) AI
// component (+0x240) has a positive count at +0x147f8, sets the attack's state at +0x30 to 3, clears
// +0x58 and hands the owner to the lazily created manager's member at +0x138 (fn_710059d3ec);
// otherwise hands the owner to the AI component's fn_710059aa2c. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CGooplotAIComponent.h"
#include "game/UnkManager_71726af468.h"

struct UnkGooplotAI_b55 {
    u8 pad_0x0[0x147f8];
    s32 field_0x147f8;
};

struct UnkGooplotOwner_b55 {
    u8 pad_0x0[0x240];
    UnkGooplotAI_b55* ai;
};

struct UnkGooplotJumpAttack_b55 {
    u8 pad_0x0[8];
    UnkGooplotOwner_b55* owner;
    u8 pad_0x10[0x20];
    u32 state;
    u8 pad_0x34[0x24];
    u32 field_0x58;
};

void fn_710059d3ec(void* member, UnkGooplotOwner_b55* owner);

void fn_7100595ca4(UnkGooplotJumpAttack_b55* self) {
    UnkGooplotAI_b55* ai = self->owner->ai;
    if (ai->field_0x147f8 <= 0) {
        fn_710059aa2c(ai);
    } else {
        self->state = 3;
        self->field_0x58 = 0;
        UnkManager_71726af468* manager = g_unk_71726af468;
        if (!manager) {
            manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
            fn_7100da7e2c(manager);
            g_unk_71726af468 = manager;
        }
        fn_710059d3ec(manager->pad_0x0 + 0x138, self->owner);
    }
}
