// Per-frame step of a component: runs the base step, then when the owner (+0x18) has a target at +0x298
// hands a default parameter block with its flag at +0x30 set to fn_71006f4b0c, with 1.
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"

struct UnkOwner {
    u8 pad_0x0[0x298];
    void* field_0x298;
};

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
};

void fn_71003e4ec8(UnkComponent* self) {
    fn_7100394bd4(self);
    if (self->field_0x18->field_0x298) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x34 = 5;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x18 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.field_0x1c = 1;
        params.field_0x30 = 1;
        fn_71006f4b0c(self->field_0x18->field_0x298, &params, 1);
    }
}
