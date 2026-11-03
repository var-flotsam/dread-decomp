// Unit 289 (tools/tu_map.py: 0x710020b8dc-0x710020c38c). Names and layouts are placeholders.
#include "types.h"

// What +0x108 points to.
struct UnkUnit289Target_b50;

struct UnkUnit289_b50 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;  // enabled
    bool field_0xfa;
    bool field_0xfb;  // a flush is pending
    u8 pad_0xfc[0xc];
    UnkUnit289Target_b50* field_0x108;
};
static_assert(__builtin_offsetof(UnkUnit289_b50, field_0x108) == 0x108, "layout");

void fn_71001c3edc(UnkUnit289Target_b50* target);

// False when disabled; clears +0xfa and runs a pending flush of +0x108 (then reports whether it is
// still enabled).
bool fn_710020bac4(UnkUnit289_b50* self) {
    if (!self->field_0xf9)
        return false;
    if (self->field_0xfa) {
        self->field_0xfa = false;
        if (!self->field_0xfb)
            return true;
    } else if (!self->field_0xfb) {
        return true;
    }
    fn_71001c3edc(self->field_0x108);
    self->field_0xfb = false;
    return self->field_0xf9;
}

