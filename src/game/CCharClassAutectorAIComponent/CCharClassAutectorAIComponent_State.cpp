// CCharClassAutectorAIComponent's unit (tools/tu_map.py unit 562): a state code from the actor's
// component at +0x2f8: 4 without one, else 3 or 4 by fn_710092b6f8. Names are placeholders.
#include "types.h"

bool fn_710092b6f8(void* component);

struct UnkActor562 {
    u8 pad_0x0[0x2f8];
    void* component;  // 0x2f8
};

u32 fn_71003bd180(void* self, void* arg, UnkActor562* actor) {
    if (actor == nullptr || actor->component == nullptr) {
        return 4;
    }
    return fn_710092b6f8(actor->component) ? 3 : 4;
}
