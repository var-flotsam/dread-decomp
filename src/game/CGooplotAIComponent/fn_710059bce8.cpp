// CGooplotAIComponent (tools/tu_map.py unit 851): the damage handler. After the base handler
// (fn_7101105e44), state 0 scales the hit (fn_71008c1538) when the owner is attacking and its
// movement is in mode 3; state 1 drops unflagged hits or clears a flag. Names and layouts are
// placeholders.
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkGooplotEvent_l59bce8 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x40];
    f32 damage;  // 0xf8
};

struct UnkGooplotOwner_l59bce8 {
    u8 pad_0x0[0x218];
    void* movement;  // 0x218
    u8 pad_0x220[0x58];
    void* attack;  // 0x278
};

struct UnkGooplotState_l59bce8 {
    u8 pad_0x0[0x6f0];
    u32 mode;  // 0x6f0
};

struct UnkGooplotAI_l59bce8 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l59bce8* owner;  // 0x30
    u8 pad_0x38[0x146d9 - 0x38];
    bool field_0x146d9;
    u8 pad_0x146da[0x1];
    bool field_0x146db;
    u8 pad_0x146dc[0x147f0 - 0x146dc];
    UnkGooplotState_l59bce8* state;  // 0x147f0
};
static_assert(__builtin_offsetof(UnkGooplotAI_l59bce8, state) == 0x147f0, "state");

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
u32 fn_710059bce8(UnkGooplotAI_l59bce8* self, UnkGooplotEvent_l59bce8* event) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    switch (self->state->mode) {
    case 1: {
        void* movement = self->owner ? self->owner->movement : nullptr;
        if (self->field_0x146db) {
            if (!(event->flags & 0x2018260))
                return 2;
        } else if (movement && fn_71010cc3a4(movement) != 3) {
            self->field_0x146d9 = false;
        }
        break;
    }
    case 0: {
        UnkGooplotOwner_l59bce8* owner = self->owner;
        if (!owner || !owner->attack)
            break;
        void* movement = owner->movement;
        bool attacking = fn_7101136308(owner->attack);
        if (movement && attacking && fn_71010cc3a4(movement) == 3 && (event->flags & 0x20e))
            fn_71008c1538(reinterpret_cast<UnkRequest*>(event), event->damage * 10.0f);
        break;
    }
    }
    return result;
}
