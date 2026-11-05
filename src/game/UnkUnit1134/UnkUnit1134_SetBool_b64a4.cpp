// Unit 1134 of tools/tu_map.py (no name hint). fn_710070d778: the bool sibling of fn_710070d6f8
// (UnkUnit1134_SetFlags_b49a3.cpp): stores the flag at +0x174 and, when there is one, at +0x58 and
// +0xa0 of the object at +0x28. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit1134Target_b64a4 {
    u8 pad_0x0[0x58];
    bool field_0x58;
    u8 pad_0x59[0x47];
    bool field_0xa0;
};

struct UnkUnit1134_b64a4 {
    u8 pad_0x0[0x28];
    UnkUnit1134Target_b64a4* target;  // 0x28
    u8 pad_0x30[0x144];
    bool field_0x174;
};

void fn_710070d778(UnkUnit1134_b64a4* self, bool value) {
    self->field_0x174 = value;
    if (UnkUnit1134Target_b64a4* target = self->target) {
        target->field_0x58 = value;
        target->field_0xa0 = value;
    }
}
