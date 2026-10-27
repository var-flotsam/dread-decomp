// Per-frame step of a component: runs the base step, clears the flag at +0x7c, then hands a default
// parameter block with its flag at +0x30 set to fn_71006f4b0c, with the target at +0x38 and 1.
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"

struct UnkComponent {
    u8 pad_0x0[0x38];
    void* field_0x38;
    u8 pad_0x40[0x3c];
    u8 field_0x7c;
};

void fn_7100384974(UnkComponent* self) {
    fn_7100394bd4(self);
    self->field_0x7c = 0;
    UnkParams params;
    __builtin_memset(params.field_0x8, 0, 13);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    params.field_0x30 = 1;
    fn_71006f4b0c(self->field_0x38, &params, 1);
}
