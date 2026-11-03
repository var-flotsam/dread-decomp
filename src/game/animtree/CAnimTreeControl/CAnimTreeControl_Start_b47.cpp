// animtree::CAnimTreeControl unit (tools/tu_map.py unit 400): with a positive time at +0x34, starts once
// (flag +0x4c): count 1 at +0x38, the time kept at +0x40, and the flag at +0x44 set unless the time is within
// 0.001 of zero; otherwise clears the target's +0x8 and that flag. Names are placeholders.
#include "types.h"

struct UnkAnimTreeTarget_b47 {
    u8 pad_0x0[0x8];
    u32 field_0x8;
};

struct UnkAnimTreeControl_b47 {
    u8 pad_0x0[0x28];
    UnkAnimTreeTarget_b47* target;  // 0x28
    u8 pad_0x30[0x4];
    f32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    f32 field_0x40;
    bool field_0x44;
    u8 pad_0x45[0x7];
    bool field_0x4c;
};

static inline bool IsNearZero_b47(f32 v) { return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f; }

void fn_71002d6150(UnkAnimTreeControl_b47* self) {
    f32 t = self->field_0x34;
    if (t > 0.0f) {
        if (!self->field_0x4c) {
            self->field_0x4c = true;
            self->field_0x38 = 1;
            self->field_0x3c = 0;
            self->field_0x40 = t;
            if (!IsNearZero_b47(t))
                self->field_0x44 = true;
        }
    } else {
        self->target->field_0x8 = 0;
        self->field_0x44 = false;
    }
}
