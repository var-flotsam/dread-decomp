// tools/tu_map.py unit 816 (no class hint): changes the state at +0x2c; entering state 1 sets +0x5c
// to 1 and copies +0x38 to +0x60. Names are placeholders.
#include "types.h"

struct UnkStateHolder816 {
    u8 pad_0x0[0x2c];
    s32 state;  // 0x2c
    u8 pad_0x30[0x8];
    s32 field_0x38;
    u8 pad_0x3c[0x20];
    s32 field_0x5c;
    s32 field_0x60;
};

void fn_7100572698(UnkStateHolder816* self, s32 state) {
    if (self->state == state) {
        return;
    }
    self->state = state;
    if (state == 1) {
        self->field_0x5c = 1;
        self->field_0x60 = self->field_0x38;
    }
}
