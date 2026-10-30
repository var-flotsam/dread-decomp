// A step in the CSpawnGroupComponent unit (unit 2703 of tools/tu_map.py): counts the word at +0x1a4
// down; once it has run out, while the flag at +0x1a0 is set and the value at +0x180 is positive, it
// and the word at +0x17c are reset to zero. Names and layouts are placeholders.
#include "types.h"

struct UnkSpawnGroup_710111f5e0 {
    u8 pad_0x0[0x17c];
    s32 field_0x17c;
    f32 field_0x180;
    u8 pad_0x184[0x1c];
    bool field_0x1a0;
    u8 pad_0x1a1[3];
    s32 field_0x1a4;
};

// NON_MATCHING(minor): the original loads the flag before testing the count and joins the two tests
// with a conditional compare; Clang branches on the count first. The behaviour is the same.
void fn_710111f5e0(UnkSpawnGroup_710111f5e0* self) {
    if (--self->field_0x1a4 <= 0 && self->field_0x1a0 && self->field_0x180 > 0.0f) {
        self->field_0x1a4 = 0;
        self->field_0x17c = 0;
    }
}
