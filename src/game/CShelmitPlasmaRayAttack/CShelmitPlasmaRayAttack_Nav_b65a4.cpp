// CShelmitPlasmaRayAttack's unit (tools/tu_map.py): sets the flag at +0x71 and, when the owner (+0x8) has a
// navigation object (+0x298) and the state at +0x30 is at most 6, hands it a kind-5 parameter block
// (game_s2/UnkParams.h) with the byte at +0x3d set. Names and layouts are placeholders.
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkPlasmaRayOwner_b65a4 {
    u8 pad_0x0[0x298];
    void* field_0x298;  // the navigation object
};

struct UnkPlasmaRayAttack_b65a4 {
    u8 pad_0x0[0x8];
    UnkPlasmaRayOwner_b65a4* owner;  // 0x8
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u8 pad_0x34[0x3d];
    bool field_0x71;
};

void fn_71006b6dc8(UnkPlasmaRayAttack_b65a4* self) {
    void* nav = self->owner->field_0x298;
    self->field_0x71 = true;
    u32 state = self->field_0x30;
    if (nav && state <= 6) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        *reinterpret_cast<u16*>(&params.pad_0x3c) = 0x100;
        fn_71006f4b0c(nav, &params, 1);
    }
}
