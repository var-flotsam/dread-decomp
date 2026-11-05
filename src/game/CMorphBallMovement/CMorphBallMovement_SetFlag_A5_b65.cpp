// CMorphBallMovement's unit: a bool setter at +0x23b0 that writes only on change. Names are placeholders.
#include "types.h"

struct UnkMorphBallMovement_A5_b65 {
    u8 pad_0x0[0x23b0];
    bool field_0x23b0;
};

void fn_710094a49c(UnkMorphBallMovement_A5_b65* self, bool value) {
    if (value != self->field_0x23b0)
        self->field_0x23b0 = value;
}
