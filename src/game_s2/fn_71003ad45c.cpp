// Unless the object at +0x14908 has its flag at +0x50c set, and when the value at +0x1140 is
// positive and the flag at +0x50e is set, stores the value at +0x14804 (-1 when fn_71003ad4dc
// accepts it).
#include "types.h"

struct UnkSubject {
    u8 pad_0x0[0x50c];
    u8 field_0x50c;
    u8 pad_0x50d;
    u8 field_0x50e;
};

struct UnkSelf {
    u8 pad_0x0[0x1140];
    s32 field_0x1140;
    u8 pad_0x1144[0x136c0];
    s32 field_0x14804;
    u8 pad_0x14808[0x100];
    UnkSubject* field_0x14908;
};

bool fn_71003ad4dc(UnkSelf* self, f32 value, s32 index);

void fn_71003ad45c(UnkSelf* self) {
    if (self->field_0x14908->field_0x50c)
        return;
    s32 value = self->field_0x1140;
    if (value >= 1 && self->field_0x14908->field_0x50e) {
        if (fn_71003ad4dc(self, -1.0f, value))
            self->field_0x14804 = -1;
        else
            self->field_0x14804 = value;
    }
}
