// Gets the value (+0x54) of the target the holder at +0x147e0 points at, reporting whether there is one.
// In state 1 it first builds a counted reference with fn_710023b580 on the member at +0x147b0, hands it
// to the holder (fn_7100234f78) and moves to state 2.
#include "game_s2/UnkWeakHandle.h"

struct UnkTarget {
    u8 pad_0x0[0x54];
    u32 field_0x54;
};

struct UnkTargetHolder {
    u8 pad_0x0[0x10];
    UnkTarget* field_0x10;
};

struct UnkHolder {
    UnkTargetHolder* field_0x0;
};

struct UnkComponent {
    u8 pad_0x0[0x147b0];
    u8 field_0x147b0[0x28];
    u32 field_0x147d8;
    u8 pad_0x147dc[4];
    UnkHolder field_0x147e0;
};

UnkRefPtr fn_710023b580(void* member);
void fn_7100234f78(UnkHolder* holder, UnkRefPtr* reference);

bool fn_71003b0224(UnkComponent* self, u32* out) {
    if (self->field_0x147d8 == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x147b0);
            fn_7100234f78(&self->field_0x147e0, &reference);
        }
        self->field_0x147d8 = 2;
    }
    if (self->field_0x147e0.field_0x0 && self->field_0x147e0.field_0x0->field_0x10) {
        *out = self->field_0x147e0.field_0x0->field_0x10->field_0x54;
        return true;
    }
    return false;
}
