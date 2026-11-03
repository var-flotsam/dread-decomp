// CPlayerMovement unit (2689 in tools/tu_map.py): setting the flag at +0x7e4 and, when it changed,
// notifying the object through its vtable slot 0x3f8. Names are placeholders.
#include "types.h"

struct UnkPlayerMovement {
    void** vtable;
    u8 pad_0x8[0x7dc];
    bool field_0x7e4;

    void vf_0x3f8() { reinterpret_cast<void (*)(UnkPlayerMovement*)>(vtable[0x3f8 / 8])(this); }
};

void fn_71010d7c8c(UnkPlayerMovement* self, bool value) {
    bool old = self->field_0x7e4;
    self->field_0x7e4 = value;
    if (old != value)
        self->vf_0x3f8();
}
