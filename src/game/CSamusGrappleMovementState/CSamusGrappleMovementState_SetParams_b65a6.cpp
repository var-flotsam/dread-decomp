// CSamusGrappleMovementState's unit: copies five words into the state at +0xb74. Names are placeholders.
#include "types.h"

struct UnkGrappleParams_b65a6 {
    u32 words[5];
};

struct UnkGrappleState_b65a6 {
    u8 pad_0x0[0xb74];
    UnkGrappleParams_b65a6 params;
};

void fn_710099c920(UnkGrappleState_b65a6* self, const UnkGrappleParams_b65a6* params) { self->params = *params; }
