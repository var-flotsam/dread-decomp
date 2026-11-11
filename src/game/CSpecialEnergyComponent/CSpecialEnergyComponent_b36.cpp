// Functions in the CSpecialEnergyComponent unit (tools/tu_map.py). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1501.h"
#include "types.h"

struct UnkEnergyHolder {
    u8 pad_0x0[0x118];
    f32 energy;  // 0x118
};

struct UnkSpecialEnergyActor {
    u8 pad_0x0[0x1f0];
    UnkEnergyHolder* holder;  // 0x1f0
    u8 pad_0x1f8[0x10];
    void* field_0x208;  // tested with fn_71009e1390
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkSpecialEnergyComponent {
    u8 pad_0x0[0x30];
    UnkSpecialEnergyActor* actor;  // 0x30
};

// True when the owner is in a CCharClassMakingNoiseTrack track (only asked when fn_71009e1390
// holds for its +0x208), else whether its energy holder has more than FLT_EPSILON left.
bool fn_7100a1f84c(UnkSpecialEnergyComponent* self) {
    if (self->actor->field_0x208 && fn_71009e1390(self->actor->field_0x208)) {
        if (UnkAnimationComponent* animation = self->actor->animation)
            return fn_710117b348(animation, fn_7100c3d378());
    }
    if (UnkEnergyHolder* holder = self->actor->holder)
        return holder->energy > 1.1920929e-07f;
    return false;
}
