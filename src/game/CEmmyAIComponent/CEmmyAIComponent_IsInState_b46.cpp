// CEmmyAIComponent's unit (tools/tu_map.py unit 811): true when fn_71005436e8 holds, the flag at
// +0x27720 is clear and the state at +0x29ef0 is 1. Names and layouts are placeholders.
#include "types.h"

struct UnkEmmyAIComponent_b46a1 {
    u8 pad_0x0[0x27720];
    bool field_0x27720;
    u8 pad_0x27721[0x29ef0 - 0x27721];
    s32 field_0x29ef0;
};

bool fn_71005436e8(UnkEmmyAIComponent_b46a1* self);

bool fn_7100557188(UnkEmmyAIComponent_b46a1* self) {
    if (fn_71005436e8(self) && !self->field_0x27720 && self->field_0x29ef0 == 1)
        return true;
    return false;
}
