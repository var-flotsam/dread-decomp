// CKraidNailMovementComponent unit (tools/tu_map.py, the name from reflection): calls fn_7100ac4184
// while the value at +0x5d0 is still -1. Names are placeholders.
#include "types.h"

struct UnkKraidNailMovement_b64a4 {
    u8 pad_0x0[0x5d0];
    f32 field_0x5d0;
};

void fn_7100ac4184(UnkKraidNailMovement_b64a4* self);

void fn_7100606934(UnkKraidNailMovement_b64a4* self) {
    if (self->field_0x5d0 == -1.0f)
        fn_7100ac4184(self);
}
