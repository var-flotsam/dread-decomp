// CCharacterMovement (tools/tu_map.py; the name from reflection): the field at +0x68 of the object
// the virtual at slot 0x2d0 returns, or 0 when it returns nothing. Names are placeholders.
#include "types.h"

struct UnkMovementTarget_b45a2 {
    u8 pad_0x0[0x68];
    u64 field_0x68;
};

struct UnkCharacterMovement_b45a2 {
    void** vtable;
    UnkMovementTarget_b45a2* vf_0x2d0() {
        return reinterpret_cast<UnkMovementTarget_b45a2* (*)(UnkCharacterMovement_b45a2*)>(vtable[0x2d0 / 8])(this);
    }
};

u64 fn_7100979ecc(UnkCharacterMovement_b45a2* self) {
    UnkMovementTarget_b45a2* t = self->vf_0x2d0();
    return t ? t->field_0x68 : 0;
}
