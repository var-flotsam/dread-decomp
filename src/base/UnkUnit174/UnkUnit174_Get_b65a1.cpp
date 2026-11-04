// Unit 174 of tools/tu_map.py (no reflected class): the word at +0x10 of an object, 0 without one.
// Names are placeholders.
#include "types.h"

struct UnkUnit174Object_b65a1 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

u32 fn_7100154260(UnkUnit174Object_b65a1* self) {
    return self ? self->field_0x10 : 0;
}
