// True when the owner's attack has less than three of its counter left after the elapsed value for
// the ultimate recharge hash is taken off: (fn_7101177da8 count) - (fn_7101177ea0 elapsed) < 3.
#include "types.h"

struct UnkAttack;

struct UnkOwner {
    u8 pad_0x0[0x220];
    UnkAttack* field_0x220;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
};

f32 fn_7101177ea0(UnkAttack* attack, u64 hash);
s32 fn_7101177da8(UnkAttack* attack, u64 hash);

bool fn_7100474ca8(UnkSelf* self) {
    if (self->field_0x30->field_0x220) {
        f32 elapsed = fn_7101177ea0(self->field_0x30->field_0x220, 0xf4061ff25a2163d6ull);
        s32 count = fn_7101177da8(self->field_0x30->field_0x220, 0xf4061ff25a2163d6ull);
        if ((f32)count - elapsed < 3.0f)
            return true;
    }
    return false;
}
