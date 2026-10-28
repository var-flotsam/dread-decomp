// Hands a default parameter block (kind 5, with the two bytes at +0x38 set) to fn_71006f4b0c, with the
// target at +0x298 of the owner (+0x18).
#include "game_s2/UnkParams.h"

struct UnkOwner {
    u8 pad_0x0[0x298];
    void* field_0x298;
};

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
};

void fn_71003c286c(UnkComponent* self) {
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.field_0x34 = 5;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    *reinterpret_cast<u16*>(&params.pad_0x38) = 0x101;
    fn_71006f4b0c(self->field_0x18->field_0x298, &params, 1);
}
