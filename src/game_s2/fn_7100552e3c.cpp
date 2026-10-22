// Picks one of three tunable values at random: the three floats at +0x108, +0x10c and +0x110 of the
// object the owner's accessor (vtable slot 0xd8 of the object at +0x29ee0) returns, chosen by
// rand() & 0x7fff modulo 3.
#include "types.h"

struct UnkTunables {
    u8 pad_0x0[0x108];
    f32 field_0x108;
    f32 field_0x10c;
    f32 field_0x110;
};

struct UnkProvider {
    void** vtable;
    UnkTunables* vf_0xd8() { return reinterpret_cast<UnkTunables* (*)(UnkProvider*)>(vtable[27])(this); }
};

struct UnkSelf {
    u8 pad_0x0[0x29ee0];
    UnkProvider* field_0x29ee0;
};

extern "C" int rand();

f32 fn_7100552e3c(UnkSelf* self) {
    alignas(16) f32 values[4];
    values[1] = self->field_0x29ee0->vf_0xd8()->field_0x108;
    f32* p = &values[1];
    values[2] = self->field_0x29ee0->vf_0xd8()->field_0x10c;
    values[3] = self->field_0x29ee0->vf_0xd8()->field_0x110;
    return p[(rand() & 0x7fff) % 3];
}
