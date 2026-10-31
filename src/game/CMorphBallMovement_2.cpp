// In the CMorphBallMovement unit: sets the flag at +0x2644 and, when it changes, stores `value` at
// +0x2648; clearing the flag also clears +0x2530 and stores -1 at +0x2648. Names and layouts are
// placeholders.
#include "types.h"

struct UnkMorphBallMovement {
    u8 pad_0x0[0x2530];
    u32 field_0x2530;
    u8 pad_0x2534[0x110];
    bool field_0x2644;
    f32 field_0x2648;
};

void fn_7100947298(UnkMorphBallMovement* self, bool enable, f32 value) {
    if (self->field_0x2644 != enable) {
        self->field_0x2644 = enable;
        self->field_0x2648 = value;
        if (!enable) {
            self->field_0x2530 = 0;
            self->field_0x2648 = -1.0f;
        }
    }
}
