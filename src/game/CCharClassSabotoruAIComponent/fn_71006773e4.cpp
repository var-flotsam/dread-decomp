// CCharClassSabotoruAIComponent's unit (tools/tu_map.py unit 999): whether the actor's CSabotoruAIComponent (its
// AI component at +0x240, cast without a null check, as in the original) is in state 1 (+0x14778); while its
// frost-bite component (+0x2c8) is frozen (fn_71007f9f20) the answer is whether it is thawing (fn_71007f9fd0).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFrozenComponent.h"
#include "types.h"

struct UnkFrostbiteComponent;

struct UnkSabotoruAI_l6773e4 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x14778 - 0x8];
    s32 state;  // 0x14778
};

struct UnkSabotoruOwner_l6773e4 {
    u8 pad_0x0[0x240];
    UnkSabotoruAI_l6773e4* ai;
    u8 pad_0x248[0x2c8 - 0x248];
    UnkFrostbiteComponent* frostbite;
};

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
bool fn_71006773e4(void* self, UnkSabotoruOwner_l6773e4* owner) {
    UnkSabotoruAI_l6773e4* ai = owner->ai;
    UnkSabotoruAI_l6773e4* sabotoru = fn_710009d50c(ai->vf_0x0(), fn_7100677dd4()) ? ai : nullptr;
    UnkFrostbiteComponent* frostbite = owner->frostbite;
    bool busy = sabotoru->state == 1;
    if (frostbite && busy && fn_71007f9f20(frostbite))
        return fn_71007f9fd0(frostbite);
    return busy;
}
