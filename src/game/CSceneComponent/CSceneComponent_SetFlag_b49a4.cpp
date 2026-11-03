// CSceneComponent unit (tools/tu_map.py): stores a flag at +0x44 when it changes and, when the
// owner's byte at +0x1b3 is set, forwards it to the virtual at +0x168. Names are placeholders.
#include "types.h"

struct UnkSceneOwner_b49a4 {
    u8 pad_0x0[0x1b3];
    u8 field_0x1b3;
};

struct UnkSceneComponent_b49a4 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkSceneOwner_b49a4* owner;  // +0x18
    u8 pad_0x20[0x24];
    bool flag;  // +0x44
};

void fn_71009f19b0(UnkSceneComponent_b49a4* self, bool flag) {
    if (self->flag == flag)
        return;
    self->flag = flag;
    if (self->owner->field_0x1b3) {
        typedef void (*Fn)(UnkSceneComponent_b49a4*, bool);
        ((Fn)self->vtable[0x168 / 8])(self, flag);
    }
}
