// CMorphBallMovement unit (tools/tu_map.py): runs the base reset (fn_71010d7b88) and clears the
// morph-ball state at +0x23c4, +0x25f0, +0x2608, +0x2620 and +0x2631. Names are placeholders.
#include "types.h"

struct UnkMorphBallMovement_b49a4 {
    u8 pad_0x0[0x23c4];
    u32 field_0x23c4[2];
    u8 pad_0x23cc[0x25f0 - 0x23cc];
    u32 field_0x25f0;
    u8 pad_0x25f4[0x2608 - 0x25f4];
    u8 field_0x2608;
    u8 pad_0x2609[0x2620 - 0x2609];
    u32 field_0x2620;
    u8 pad_0x2624[0x2631 - 0x2624];
    u8 field_0x2631;
};

void fn_71010d7b88(UnkMorphBallMovement_b49a4* self, bool flag);

void fn_710094a398(UnkMorphBallMovement_b49a4* self, bool flag) {
    fn_71010d7b88(self, flag);
    self->field_0x23c4[0] = 0;
    self->field_0x23c4[1] = 0;
    self->field_0x25f0 = 0;
    self->field_0x2608 = 0;
    self->field_0x2620 = 0;
    self->field_0x2631 = 0;
}
