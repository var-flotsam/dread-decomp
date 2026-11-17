// CTakumakuAIComponent unit (tools/tu_map.py unit 1079): the base check (fn_7101110cb0), else, with the
// owner's movement (+0x218), what fn_71010cab0c says of it. Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "types.h"

// As src/game/UnkTU2702/UnkTU2702_Misc_ovb0143.cpp defines it.
struct UnkMoveSelf_b0143;
bool fn_7101110cb0(UnkMoveSelf_b0143* self);

struct UnkTakumakuOwner_l6d5938 {
    u8 pad_0x0[0x218];
    void* movement;
};

struct UnkTakumakuAI_l6d5938 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner_l6d5938* owner;
};

bool fn_71006d5938(UnkTakumakuAI_l6d5938* self) {
    bool result = fn_7101110cb0(reinterpret_cast<UnkMoveSelf_b0143*>(self));
    UnkTakumakuOwner_l6d5938* owner = self->owner;
    if (owner && owner->movement)
        result = result || fn_71010cab0c(owner->movement);
    return result;
}
