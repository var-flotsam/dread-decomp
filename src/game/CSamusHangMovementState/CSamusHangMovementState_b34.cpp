#include "types.h"

struct CSamusHangMovementState {
    u8 unk_0x0[0x188];
    u8 listener[0x38];  // 0x188
    bool field_0x1c0;
};

bool fn_71009a2a84(CSamusHangMovementState* self);

// Listener callback at +0x188: marks the state when fn_71009a2a84 refuses.
void fn_71009a2b64(void* listener) {
    CSamusHangMovementState* self =
        reinterpret_cast<CSamusHangMovementState*>(static_cast<char*>(listener) - 0x188);
    asm("" : "+r"(self));
    if (!fn_71009a2a84(self))
        self->field_0x1c0 = true;
}
