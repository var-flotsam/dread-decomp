// CAnimationComponent unit (tools/tu_map.py): whether any entry behind +0x40 has one of a mask's bits at
// +0x94 of its object. Names and layouts are placeholders.
#include "types.h"

struct UnkAnimMaskTarget_b47a3 {
    u8 unk_0x0[0x94];
    u32 flags;
};

struct UnkAnimMaskSlot_b47a3 {
    u8 unk_0x0[0x70];
    UnkAnimMaskTarget_b47a3* target;
};

struct UnkAnimMaskSlots_b47a3 {
    u8 unk_0x0[0xf0];
    UnkAnimMaskSlot_b47a3** slots;
    u8 unk_0xf8[8];
    u32 count;
};

struct UnkAnimMaskHolder_b47a3 {
    u8 unk_0x0[0x40];
    UnkAnimMaskSlots_b47a3* slots;
};

bool fn_7101179cbc(UnkAnimMaskHolder_b47a3* self, const u32* bits) {
    UnkAnimMaskSlots_b47a3* s = self->slots;
    for (UnkAnimMaskSlot_b47a3** it = s->slots; it != s->slots + s->count; ++it) {
        if ((*it)->target->flags & *bits)
            return true;
    }
    return false;
}
