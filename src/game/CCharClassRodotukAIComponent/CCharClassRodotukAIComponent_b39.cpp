// CCharClassRodotukAIComponent's unit (tools/tu_map.py unit 991): the same state code as fn_710041b180
// (src/game/CCharClassRobotAIComponent/CCharClassRobotAIComponent_State.cpp): 4 without the actor's
// component at +0x2f8, else 5 or 4 by fn_710092b6f8. Names are placeholders.
#include "types.h"

bool fn_710092b6f8(void* component);

struct UnkActor991 {
    u8 pad_0x0[0x2f8];
    void* component;  // 0x2f8
};

u32 fn_710066fbc0(void* self, void* arg, UnkActor991* actor) {
    if (actor != nullptr && actor->component != nullptr) {
        return fn_710092b6f8(actor->component) ? 5u : 4u;
    }
    u32 none = 4;
    asm("" : "+r"(none));
    return none;
}
