// CEmmyAIComponent's unit (tools/tu_map.py unit 811): 0 while the timer at +0x29418 runs
// (fn_710002fb70) or the flag at +0x29f26 is set, else the base AI component's value
// (fn_710110f6d8). Names and layouts are placeholders.
#include "types.h"

bool fn_710002fb70(void* timer);

struct UnkEmmyAI_b46a1 {
    u8 pad_0x0[0x29418];
    u8 timer[0x29f26 - 0x29418];
    bool field_0x29f26;
};

f32 fn_710110f6d8(UnkEmmyAI_b46a1* self);

f32 fn_71005578e0(UnkEmmyAI_b46a1* self) {
    if (fn_710002fb70(self->timer) || self->field_0x29f26)
        return 0.0f;
    return fn_710110f6d8(self);
}
