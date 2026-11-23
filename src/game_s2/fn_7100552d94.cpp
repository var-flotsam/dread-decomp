// Picks one of three tunable values at random: the three floats at +0xf4, +0xf8 and +0xfc of the
// object the owner's accessor (vtable slot 0xd8 of the object at +0x29ee0) returns, chosen by
// rand() & 0x7fff modulo 3.
#include "game/UnkTU811.h"
#include "game_s2/UnkProvider.h"
#include "game_s2/UnkTunables.h"
#include "types.h"

struct UnkSelf {
    u8 pad_0x0[0x29ee0];
    UnkProvider* field_0x29ee0;
};

extern "C" int rand();

f32 fn_7100552d94(UnkSelf* self) {
    alignas(16) f32 values[4];
    values[1] = self->field_0x29ee0->vf_0xd8()->field_0xf4;
    f32* p = &values[1];
    values[2] = self->field_0x29ee0->vf_0xd8()->field_0xf8;
    values[3] = self->field_0x29ee0->vf_0xd8()->field_0xfc;
    return p[(rand() & 0x7fff) % 3];
}
