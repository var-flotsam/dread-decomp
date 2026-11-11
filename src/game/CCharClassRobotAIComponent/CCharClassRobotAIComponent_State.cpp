// CCharClassRobotAIComponent's unit (tools/tu_map.py unit 621): a state code from the actor's
// component at +0x2f8: 4 without one, else 5 or 4 by fn_710092b6f8. Names are placeholders.
#include "game/CCharClassRobotAIComponent.h"
#include "game/CMeleeComponent.h"
#include "types.h"

struct UnkActor621 {
    u8 pad_0x0[0x2f8];
    void* component;  // 0x2f8
};

u32 fn_710041b180(void* self, void* arg, UnkActor621* actor) {
    if (actor != nullptr && actor->component != nullptr) {
        return fn_710092b6f8(actor->component) ? 5u : 4u;
    }
    u32 none = 4;
    asm("" : "+r"(none));
    return none;
}
