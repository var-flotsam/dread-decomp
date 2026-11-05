// CSamusMovement's unit: sets the flag at +0x22f9 only when it changes. Names are placeholders.
#include "types.h"

struct UnkSamusMovement_b65a6 {
    u8 pad_0x0[0x22f9];
    bool flag;
};

void fn_71009e9b10(UnkSamusMovement_b65a6* self, bool value) {
    if (value != self->flag)
        self->flag = value;
}
