// Unit 250 of tools/tu_map.py (no reflected class): three 16-bit fields of the object returned as
// floats (one unsigned at +0x9c, two signed at +0xa0 and +0xa2). Names are placeholders.
#include "types.h"

struct UnkUnit250Shorts_b65a1 {
    u8 pad_0x0[0x9c];
    u16 field_0x9c;
    u8 pad_0x9e[0x2];
    s16 field_0xa0;
    s16 field_0xa2;
};

f32 fn_71001bd59c(UnkUnit250Shorts_b65a1* self) { return self->field_0x9c; }
f32 fn_71001bd634(UnkUnit250Shorts_b65a1* self) { return self->field_0xa0; }
f32 fn_71001bd644(UnkUnit250Shorts_b65a1* self) { return self->field_0xa2; }
