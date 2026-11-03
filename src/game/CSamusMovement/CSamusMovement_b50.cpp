// CSamusMovement unit (tools/tu_map.py): fn_71009e6940, whether the owner's (+0x30) ghost aura
// ability (CAbilityGhostAura, in its ability set at +0x2e0) is in state 3 to 6 (fn_7100309568), as
// fn_7100df5630 asks of the player. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAbility {
    virtual const CClass* vf_0x0();
};
struct UnkAbilitySet;
UnkAbility* fn_71003013ec(UnkAbilitySet* set, u64 id);
u64 fn_7100308c74();
u32 fn_7100309568(UnkAbility* ability);

struct UnkMovementOwner_b50a5 {
    u8 unk_0x0[0x2e0];
    UnkAbilitySet* abilities;
};

struct UnkSamusMovement_b50a5 {
    u8 unk_0x0[0x30];
    UnkMovementOwner_b50a5* owner;
};

bool fn_71009e6940(UnkSamusMovement_b50a5* self) {
    UnkAbilitySet* abilities = self->owner->abilities;
    if (!abilities)
        return false;
    UnkAbility* ability = fn_71003013ec(abilities, fn_7100308c74());
    if (!ability)
        return false;
    if (!fn_710009d50c(ability->vf_0x0(), fn_7100306914()))
        return false;
    return fn_7100309568(ability) - 3 < 4;
}
