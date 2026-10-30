// From the CAimComponent unit: whether the actor's animation component (+0x220 of the object at +0x30)
// has a track of the class fn_7100d03e08 gives, asked only when the object at +0x50 is in state 1 and
// the flag at +0x360 is set. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

struct UnkAimState {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkAimActor {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkAimComponent {
    u8 pad_0x0[0x30];
    UnkAimActor* actor;  // 0x30
    u8 pad_0x38[0x18];
    UnkAimState* aim;  // 0x50
    u8 pad_0x58[0x308];
    bool field_0x360;
};

bool fn_710034f8dc(UnkAimComponent* self) {
    if (self->aim && self->aim->state == 1 && self->field_0x360) {
        UnkAnimationComponent* animation = self->actor->animation;
        return fn_710117b348(animation, fn_7100d03e08());
    }
    return false;
}
