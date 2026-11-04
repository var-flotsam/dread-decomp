// behaviortree::CParticleEnableEmission unit (tools/tu_map.py, the name from reflection): the status it
// reports: 6 before it is set up (+0x3b), 5 once done (+0x38) or when it does not wait (+0x3a), else 2
// while the effect at +0x28 of the object at +0x40 is still active (fn_71002084b4). Names and layouts are
// placeholders.
#include "types.h"

// As src/game/UnkUnit285/UnkUnit285_UpdateActive_b46.cpp defines it.
struct UnkUnit285_b46a1;
bool fn_71002084b4(UnkUnit285_b46a1* self);

struct UnkParticleEmissionTarget_b66a2 {
    u8 pad_0x0[0x28];
    u8 effect[1];
};

struct UnkParticleEnableEmission_b66a2 {
    u8 pad_0x0[0x38];
    bool done;
    u8 pad_0x39;
    bool wait;
    bool started;
    u8 pad_0x3c[4];
    UnkParticleEmissionTarget_b66a2* target;
};

s32 fn_7100378590(UnkParticleEnableEmission_b66a2* self) {
    if (!self->started) {
        return 6;
    }
    if (!self->done && self->wait &&
        fn_71002084b4(reinterpret_cast<UnkUnit285_b46a1*>(self->target->effect))) {
        return 2;
    }
    return 5;
}
