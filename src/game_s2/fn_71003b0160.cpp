// Gets the value (+0x54) of the target the holder at +0x147a0 points at, reporting whether there is one.
// In state 1 it first builds a counted reference with fn_710023b580 on the member at +0x14770, hands it
// to the holder (fn_7100234f78) and moves to state 2.
#include "base/UnkRefHandle.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkTarget {
    u8 pad_0x0[0x54];
    u32 field_0x54;
};

// The weak handle the holder refers to (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkTargetHolder : base::UnkRefCounted {
    UnkTarget* field_0x10;
};

struct UnkComponent {
    u8 pad_0x0[0x14770];
    u8 field_0x14770[0x28];
    u32 field_0x14798;
    u8 pad_0x1479c[4];
    base::UnkRefHandle field_0x147a0;
};

UnkRefPtr fn_710023b580(void* member);

bool fn_71003b0160(UnkComponent* self, u32* out) {
    if (self->field_0x14798 == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x14770);
            // The reference's pointer is the handle's.
            fn_7100234f78(&self->field_0x147a0, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->field_0x14798 = 2;
    }
    UnkTargetHolder* holder = static_cast<UnkTargetHolder*>(self->field_0x147a0.ptr);
    if (holder && holder->field_0x10) {
        *out = holder->field_0x10->field_0x54;
        return true;
    }
    return false;
}
