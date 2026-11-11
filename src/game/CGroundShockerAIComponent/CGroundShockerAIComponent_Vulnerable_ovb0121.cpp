// CGroundShockerAIComponent's unit (tools/tu_map.py): fn_71005a48b0 counts down the vulnerable timer (+0x14738)
// while the owner is not frozen, then recomputes the vulnerable flag (+0x1473c) as fn_71005a4be8 does
// (src/game/CGroundShockerAIComponent/CGroundShockerAIComponent_Busy_A3_b62.cpp), calling fn_71005a4510 when it
// drops. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFrostbiteComponent;

struct UnkShockerState_ovb0121 {
    u8 pad_0x0[0x70];
    s32 kind;  // 0x70
};

struct UnkShockerOwner_ovb0121 {
    u8 pad_0x0[0x1d0];
    UnkShockerState_ovb0121* state;  // 0x1d0
    u8 pad_0x1d8[0x220 - 0x1d8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0xa0];
    UnkFrostbiteComponent* frostbite;  // 0x2c8
};

struct UnkShockerAI_ovb0121 {
    u8 pad_0x0[0x30];
    UnkShockerOwner_ovb0121* owner;
    u8 pad_0x38[0x14738 - 0x38];
    f32 vulnerable_time;  // 0x14738
    bool vulnerable;      // 0x1473c
};

void fn_71005a4510(UnkShockerAI_ovb0121* self);

static inline u32 IsVulnerable(UnkShockerAI_ovb0121* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    UnkFrostbiteComponent* frostbite = self->owner->frostbite;
    if (animation) {
        if (self->vulnerable_time > 0.001f || fn_710117b348(animation, fn_7100c368d8()))
            return true;
    }
    if (frostbite)
        return fn_71007f9f20(frostbite) && !fn_71007f9fd0(frostbite);
    return false;
}

void fn_71005a48b0(UnkShockerAI_ovb0121* self, f32 dt) {
    UnkShockerOwner_ovb0121* owner = self->owner;
    if (owner->state && owner->state->kind != 2) {
        UnkFrostbiteComponent* frostbite = owner->frostbite;
        if (!frostbite || !fn_71007f9f20(frostbite)) {
            if (self->vulnerable_time > 0.001f)
                self->vulnerable_time -= dt;
        }
    }
    bool vulnerable = IsVulnerable(self);
    if (self->vulnerable && !vulnerable)
        fn_71005a4510(self);
    self->vulnerable = vulnerable;
}
