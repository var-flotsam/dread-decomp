// CRodotukSuckAttack unit (tools/tu_map.py, the name from reflection): calls its virtual at slot 0xf0
// with 3 when the mode at +0x60 is 1, else 1. Names are placeholders.
#include "types.h"

struct UnkRodotukSuckAttack_b64a4 {
    void** vtable;
    u8 pad_0x8[0x58];
    s32 field_0x60;
};

void fn_710066c7b0(UnkRodotukSuckAttack_b64a4* self) {
    typedef void (*Fn)(UnkRodotukSuckAttack_b64a4*, u32);
    bool one = self->field_0x60 == 1;
    Fn* vtable = reinterpret_cast<Fn*>(self->vtable);
    vtable[0xf0 / 8](self, one ? 3 : 1);
}
