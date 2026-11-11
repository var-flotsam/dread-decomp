// CCharClassSabotoruAIComponent's unit (tools/tu_map.py; the name from reflection): a state code
// from the actor's component at +0x2f8 while the state at +0x600 is 1: 1 when fn_710092b6f8 holds,
// else 4 (the shape of fn_71003bd180). Names are placeholders.
#include "game/CMeleeComponent.h"
#include "types.h"

struct UnkSabotoruAI_b45a2 {
    u8 pad_0x0[0x600];
    s32 state;
};

struct UnkSabotoruActor_b45a2 {
    u8 pad_0x0[0x2f8];
    void* component;
};

u32 fn_7100677398(UnkSabotoruAI_b45a2* self, void* arg, UnkSabotoruActor_b45a2* actor) {
    if (actor != nullptr && self->state == 1 && actor->component != nullptr)
        return fn_710092b6f8(actor->component) ? 1 : 4;
    u32 none = 4;
    asm("" : "+r"(none));  // kept apart from the other return, as in the original
    return none;
}
