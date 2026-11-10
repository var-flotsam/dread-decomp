// Unit 47 (tools/tu_map.py): the rumble manager's table of players. This one sets a new strength scale
// and marks every player dirty when it changed. Names and layouts are placeholders.
#include "types.h"

struct UnkRumbleTarget_ovb0219 {
    u8 pad_0x0[0x1a3];
    u8 dirty;  // 0x1a3
};

struct UnkRumbleTable_ovb0219 {
    u8 pad_0x0[0x20a0];
    UnkRumbleTarget_ovb0219** values;  // 0x20a0
    u8 pad_0x20a8[8];
    u32 count;  // 0x20b0
    u8 pad_0x20b4[0x14];
    f32 scale;  // 0x20c8
};

void fn_710006fa8c(UnkRumbleTable_ovb0219* self, f32 scale) {
    if (self->scale == scale)
        return;
    self->scale = scale;
    UnkRumbleTarget_ovb0219** end = self->values + self->count;
    for (UnkRumbleTarget_ovb0219** p = self->values; p != end; ++p)
        (*p)->dirty = 1;
}
