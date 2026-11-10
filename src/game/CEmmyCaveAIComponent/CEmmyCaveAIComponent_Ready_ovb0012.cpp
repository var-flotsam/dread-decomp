// CEmmyCaveAIComponent's unit (tools/tu_map.py unit 782): whether the E.M.M.I. AI check fn_71005436e8
// holds while the flag at +0x27720 is clear. Names and layouts are placeholders.
#include "types.h"

// As src/game/CEmmyAIComponent/CEmmyAIComponent_IsInState_b46.cpp declares it.
struct UnkEmmyAIComponent_b46a1;
bool fn_71005436e8(UnkEmmyAIComponent_b46a1* self);

struct UnkEmmyCaveAI_b0012 {
    u8 pad_0x0[0x27720];
    bool field_0x27720;
};

bool fn_71004f0a6c(UnkEmmyCaveAI_b0012* self) {
    if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)) && !self->field_0x27720)
        return true;
    return false;
}
