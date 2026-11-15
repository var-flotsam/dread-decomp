// CShotManager::CTunableShotManager unit: clears the reset flag, marks every shot dead, zeroes the
// per-frame draw counts and runs the update with no time. Names and layouts are placeholders.
#include "types.h"

struct UnkShot_ed6bc4 {
    u8 pad_0x0[0x14a];
    bool field_0x14a;  // live
    u8 pad_0x14b[0x290 - 0x14b];
};

struct UnkShotManager_ed6bc4 {
    u8 pad_0x0[0x8];
    UnkShot_ed6bc4* shots;
    u32 shotCount;
    u8 pad_0x14[0xd8 - 0x14];
    u64 field_0xd8[5];
    u8 pad_0x100[0x1270 - 0x100];
    bool field_0x1270;
};

void fn_7100ed6c08(UnkShotManager_ed6bc4* self, f32 dt);

void fn_7100ed6bc4(UnkShotManager_ed6bc4* self) {
    self->field_0x1270 = false;
    for (UnkShot_ed6bc4 *it = self->shots, *end = self->shots + self->shotCount; it != end; ++it)
        it->field_0x14a = false;
    for (u32 i = 0; i < 5; ++i)
        self->field_0xd8[i] = 0;
    fn_7100ed6c08(self, 0.0f);
}
