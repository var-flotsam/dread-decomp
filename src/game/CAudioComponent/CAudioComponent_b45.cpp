// CAudioComponent's unit (tools/tu_map.py; the name from reflection): copies the settings block at
// +0x138 over the one at +0x118. Names and layout are placeholders.
#include "types.h"

struct UnkPair_b45a2 {
    f32 x;
    f32 y;
};

struct UnkAudioSettings_b45a2 {
    u32 field_0x0;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    UnkPair_b45a2 field_0x14;
    u32 field_0x1c;
};

struct UnkAudioComponent_b45a2 {
    u8 pad_0x0[0x118];
    UnkAudioSettings_b45a2 current;  // 0x118
    UnkAudioSettings_b45a2 saved;    // 0x138
};

void fn_710071f5ac(UnkAudioComponent_b45a2* self) {
    UnkAudioSettings_b45a2& d = self->current;
    const UnkAudioSettings_b45a2& s = self->saved;
    d.field_0x0 = s.field_0x0;
    d.field_0x4 = s.field_0x4;
    d.field_0x8 = s.field_0x8;
    d.field_0x10 = s.field_0x10;
    const UnkPair_b45a2* from = &s.field_0x14;
    asm("" : "+r"(from));  // the original addresses the unaligned pair on its own
    d.field_0x14 = *from;
    d.field_0x1c = s.field_0x1c;
}
