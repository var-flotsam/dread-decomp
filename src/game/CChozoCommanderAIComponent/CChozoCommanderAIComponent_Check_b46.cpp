// CChozoCommanderAIComponent's unit (tools/tu_map.py): while the owner's component at +0x278 is
// active (fn_7101136308) and its +0x1a0 is the AI's own entry at +0x149a8, clears the output
// flag. Names and layouts are placeholders.
#include "game/CAttackComponent.h"
#include "types.h"

struct UnkChozoCommanderSlot_b46a1 {
    u8 pad_0x0[0x1a0];
    void* field_0x1a0;
};

struct UnkChozoCommanderOwner_b46a1 {
    u8 pad_0x0[0x278];
    UnkChozoCommanderSlot_b46a1* field_0x278;
};

struct UnkChozoCommanderAI_b46a1 {
    u8 pad_0x0[0x30];
    UnkChozoCommanderOwner_b46a1* owner;
    u8 pad_0x38[0x149a8 - 0x38];
    u8 field_0x149a8[8];
};

void fn_7100453e80(UnkChozoCommanderAI_b46a1* self, void*, bool* out) {
    UnkChozoCommanderSlot_b46a1* slot = self->owner->field_0x278;
    if (slot && fn_7101136308(slot) && slot->field_0x1a0 == self->field_0x149a8)
        *out = false;
}
