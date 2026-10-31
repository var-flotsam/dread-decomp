// A test in the CDropterAIComponent unit (fn_71004ef054): whether the object at +0x14738 is in
// state 2 while the component's own state at +0x14864 is not 3. Names and layouts are
// placeholders.
#include "types.h"

struct UnkDropterTarget {
    u8 pad_0x0[0x620];
    s32 field_0x620;
};

struct UnkDropterComponent {
    u8 pad_0x0[0x14738];
    UnkDropterTarget* field_0x14738;
    u8 pad_0x14740[0x124];
    s32 field_0x14864;
};

bool fn_71004ef054(const UnkDropterComponent* self) {
    if (self->field_0x14738->field_0x620 == 2 && self->field_0x14864 != 3)
        return true;
    return false;
}
