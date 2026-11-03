#include "types.h"

struct UnkGunFlags {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
};

struct CGun {
    void** vtable;
    u8 unk_0x8[0x1871 - 0x8];
    UnkGunFlags flags;  // 0x1871
};

// Clears the first flag, runs slot 0x1d0, then clears the other two.
void fn_7100835908(CGun* self) {
    UnkGunFlags* flags = &self->flags;
    flags->field_0x0 = false;
    reinterpret_cast<void (*)(CGun*)>(self->vtable[0x1d0 / 8])(self);
    flags->field_0x1 = false;
    flags->field_0x2 = false;
}
