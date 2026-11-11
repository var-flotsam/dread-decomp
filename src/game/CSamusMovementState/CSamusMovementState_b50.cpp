// CSamusMovementState unit (tools/tu_map.py, unit 1468): fn_71009b12ec, whether the state may
// proceed: not while fn_71009e1310 holds for the object at +0x38; when the owner's (+0x30) flag
// (+0x2b0, +0x360) is set, only if its animation component (+0x220) has a CCharClassIgnoreAimTrack
// (fn_710117b348); then unless the owner's ghost aura ability (CAbilityGhostAura) is in state 2.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAbility {
    virtual const CClass* vf_0x0();
};
struct UnkAbilitySet;
bool fn_71009e1310(void* self);

struct UnkSamusFlags_b50a5 {
    u8 unk_0x0[0x360];
    bool field_0x360;
};

struct UnkSamusOwner_b50a5 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 unk_0x228[0x88];
    UnkSamusFlags_b50a5* flags;  // 0x2b0
    u8 unk_0x2b8[0x28];
    UnkAbilitySet* abilities;  // 0x2e0
};

struct UnkSamusState_b50a5 {
    u8 unk_0x0[0x30];
    UnkSamusOwner_b50a5* owner;  // 0x30
    void* field_0x38;
};

static inline UnkAbility* GetGhostAura_b50a5(UnkSamusOwner_b50a5* owner) {
    UnkAbilitySet* abilities = owner->abilities;
    if (!abilities)
        return nullptr;
    UnkAbility* ability = fn_71003013ec(abilities, fn_7100308c74());
    if (!ability)
        return nullptr;
    return fn_710009d50c(ability->vf_0x0(), fn_7100306914()) ? ability : nullptr;
}

bool fn_71009b12ec(UnkSamusState_b50a5* self) {
    if (fn_71009e1310(self->field_0x38))
        return false;
    if (self->owner->flags->field_0x360 && !fn_710117b348(self->owner->animation, fn_7100d03e08()))
        return false;
    UnkAbility* ability = GetGhostAura_b50a5(self->owner);
    if (ability && fn_7100309568(ability) == 2)
        return false;
    return true;
}
