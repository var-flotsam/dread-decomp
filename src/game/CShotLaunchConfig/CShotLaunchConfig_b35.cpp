// A constructor in the CShotLaunchConfig unit: clears 0x50-0x60 and 0x20, sets the index at +0x60 to
// -1, clears 0x64-0x94 and 0x0-0x20, then finishes with fn_71009f9dc0. Names and layouts are placeholders.
#include "game/CShotLaunchConfig.h"
#include "types.h"

struct UnkShotLaunchState {
    u64 field_0x0[4];
    u32 field_0x20;
    u8 pad_0x24[0x2c];
    u64 field_0x50[2];
    s32 field_0x60;
    u8 field_0x64[0x30];
};

void fn_71009f9d8c(UnkShotLaunchState* self) {
    __builtin_memset(self->field_0x50, 0, sizeof(self->field_0x50));
    self->field_0x20 = 0;
    self->field_0x60 = -1;
    __builtin_memset(self->field_0x64, 0, sizeof(self->field_0x64));
    __builtin_memset(self->field_0x0, 0, sizeof(self->field_0x0));
    fn_71009f9dc0(self);
}
