// CEmmyAIComponent unit (tools/tu_map.py): fn_710054aa5c answers whether the EMMI may act (flag +0x14c14 on,
// fn_710002fb70 false for the part at +0x29350, and the owner's animation (+0x220) not playing the class
// fn_7100c78d04); fn_710054bc28 picks a tunable value (+0x149b0, +0x150, +0x70 or +0xe0 into the tunables)
// by the first of four state tests that holds, else 0. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkEmmyAIView;
struct UnkEmmyAIComponent_b46a1;

struct UnkEmmyOwner2_b62a3 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkEmmyAI2_b62a3 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner2_b62a3* owner;
    u8 pad_0x38[0x14c14 - 0x38];
    bool field_0x14c14;
};

bool fn_710054aa5c(UnkEmmyAI2_b62a3* self) {
    if (!self->field_0x14c14)
        return false;
    if (fn_710002fb70(reinterpret_cast<char*>(self) + 0x29350))
        return false;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    return !fn_710117b348(animation, fn_7100c78d04());
}

f32 fn_710054bc28(UnkEmmyAI2_b62a3* self) {
    char* tunables = reinterpret_cast<char*>(self) + 0x149b0;
    asm("" : "+r"(tunables));
    if (fn_710054ad2c(reinterpret_cast<UnkEmmyAIView*>(self)))
        return *reinterpret_cast<f32*>(tunables);
    if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)))
        return *reinterpret_cast<f32*>(tunables + 0x150);
    if (fn_710054aecc(self))
        return *reinterpret_cast<f32*>(tunables + 0x70);
    if (fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self)))
        return *reinterpret_cast<f32*>(tunables + 0xe0);
    return 0.0f;
}
