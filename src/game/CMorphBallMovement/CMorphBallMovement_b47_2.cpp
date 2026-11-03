// CMorphBallMovement unit (tools/tu_map.py): sets the state flag through the base setter (fn_71010d15a0);
// when the flag at +0x170 changed, clears the timer at +0x2654 and, when it went off, takes the pair at
// +0x9c of the object at +0x2280 and raises two flags. Names and layouts are placeholders.
#include "types.h"

struct UnkMorphBallSource_b47 {
    u8 pad_0x0[0x9c];
    u32 field_0x9c;
    u32 field_0xa0;
};

struct UnkMorphBallMovement_b47 {
    u8 pad_0x0[0x170];
    bool field_0x170;
    u8 pad_0x171[0x2280 - 0x171];
    UnkMorphBallSource_b47* field_0x2280;
    u8 pad_0x2288[0x23c8 - 0x2288];
    u32 field_0x23c8;
    u32 field_0x23cc;
    u8 pad_0x23d0[7];
    bool field_0x23d7;
    bool field_0x23d8;
    u8 pad_0x23d9[0x2654 - 0x23d9];
    u32 field_0x2654;
};

void fn_71010d15a0(UnkMorphBallMovement_b47* self, bool value);

void fn_710094b030(UnkMorphBallMovement_b47* self, bool value) {
    bool old = self->field_0x170;
    fn_71010d15a0(self, value);
    if (old != self->field_0x170) {
        self->field_0x2654 = 0;
        if (!self->field_0x170) {
            UnkMorphBallSource_b47* source = self->field_0x2280;
            u32 x = source->field_0x9c;
            u32 y = source->field_0xa0;
            self->field_0x23c8 = x;
            self->field_0x23cc = y;
            self->field_0x23d7 = true;
            self->field_0x23d8 = true;
        }
    }
}
