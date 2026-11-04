// CShakernautAIComponent unit (tools/tu_map.py): after the base step (fn_7101106490), clears the
// counter at +0x14808 when `which` is the member at +0x146d0, or the one at +0x1480c when it is the
// member at +0x14738. Names and layouts are placeholders.
#include "types.h"

struct UnkShakernautAI_b64a4 {
    u8 pad_0x0[0x146d0];
    u8 field_0x146d0[0x68];
    u8 field_0x14738[0xd0];
    u32 field_0x14808;
    u32 field_0x1480c;
};

void fn_7101106490(UnkShakernautAI_b64a4* self, void* which);

void fn_71006b0e10(UnkShakernautAI_b64a4* self, void* which) {
    fn_7101106490(self, which);
    if (which == self->field_0x146d0) {
        self->field_0x14808 = 0;
        asm volatile("");  // keeps the two stores apart, as in the original
        return;
    }
    if (which == self->field_0x14738) {
        self->field_0x1480c = 0;
    }
}
