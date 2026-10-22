// Switches the owner's attack between two modes by the sign of a float read from the owner's
// object at +0x38: calls fn_7101178e08 with mode 5 for a negative value (4 otherwise) and
// fn_710117662c with the opposite mode.
#include "types.h"

struct UnkAttack;

struct UnkAmount {
    u8 pad_0x0[8];
    f32 field_0x8;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[1];
    u8 pad_0x39[0x1e7];
    UnkAttack* field_0x220;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
};

UnkAmount* fn_710002614c(void* object);
void fn_7101178e08(UnkAttack* attack, u32 mode, u32 a, u32 b, f32 rate);
void fn_710117662c(UnkAttack* attack, u32 mode, f32 rate, u32 a, u32 b, u32 c);

void fn_7100498db8(UnkSelf* self) {
    UnkOwner* owner = self->field_0x30;
    UnkAttack* attack = owner->field_0x220;
    if (attack) {
        bool negative = !(fn_710002614c(owner->field_0x38)->field_0x8 >= 0.0f);
        u32 first = negative ? 5 : 4;
        u32 second = negative ? 4 : 5;
        fn_7101178e08(attack, first, 0, 0, -1.0f);
        fn_710117662c(attack, second, -1.0f, 1, 0, 0);
    }
}
