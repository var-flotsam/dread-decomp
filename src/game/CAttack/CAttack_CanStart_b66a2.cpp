// CAttack unit (tools/tu_map.py, the name from reflection): true when the owner (+0x8) has a character
// class (+0x220), the flag at +0x14 is clear and that class is not the one fn_7100ceda9c names
// (fn_710117b348). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkAttackOwner_b66a2 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* character_class;
};

struct UnkAttack_b66a2 {
    u8 pad_0x0[0x8];
    UnkAttackOwner_b66a2* owner;
    u8 pad_0x10[4];
    u8 flag;
};

bool fn_7100362498(UnkAttack_b66a2* self) {
    UnkAnimationComponent* character_class = self->owner->character_class;
    u8 flag = self->flag;
    bool result = false;
    if ((character_class != nullptr) & (flag == 0)) {
        result = !fn_710117b348(character_class, fn_7100ceda9c());
    }
    return result;
}
