// The CEmmyAIComponent unit: fn_7100551ce4 and fn_7100551d98, two identical tests. False when the
// flag at +0x14779 is clear, when fn_710002fb70 holds for the object at +0x29350, or when the
// owner's +0x260 slot passes fn_710113d83c; otherwise true unless the owner's character class
// (+0x220) is playing a CCharClassIgnorePerceptionTrack. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CGrabComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkEmmyOwner_b63a3 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x38];
    void* field_0x260;
};

struct UnkEmmyAI_b63a3 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_b63a3* owner;
    u8 pad_0x38[0x14779 - 0x38];
    bool field_0x14779;
    u8 pad_0x1477a[0x29350 - 0x1477a];
    u8 field_0x29350[8];
};

static inline __attribute__((always_inline)) bool CanAct_b63a3(UnkEmmyAI_b63a3* self) {
    if (!self->field_0x14779)
        return false;
    if (fn_710002fb70(self->field_0x29350))
        return false;
    if (void* p = self->owner->field_0x260) {
        if (fn_710113d83c(p))
            return false;
    }
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if (!anim)
        return true;
    return !fn_710117b348(anim, fn_7100c78d04());
}

bool fn_7100551ce4(UnkEmmyAI_b63a3* self) { return CanAct_b63a3(self); }
bool fn_7100551d98(UnkEmmyAI_b63a3* self) { return CanAct_b63a3(self); }
