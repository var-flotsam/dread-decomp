// CCharClassGooplotAIComponent unit (tools/tu_map.py, the name from reflection): 2 when the actor's +0x1f0
// object has its flag at +0xf9 set or its animation (+0x220) plays a CCharClassAllowCrazyTrack
// (fn_7100ce9338), else 1. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkGooplotState_b54 {
    u8 pad_0x0[0xf9];
    bool flag;
};

struct UnkGooplotActor_b54 {
    u8 pad_0x0[0x1f0];
    UnkGooplotState_b54* state;
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
};

u32 fn_7100599230(void*, UnkGooplotActor_b54* actor) {
    if (actor->state->flag)
        return 2;
    return fn_710117b348(actor->animation, fn_7100ce9338()) ? 2 : 1;
}
