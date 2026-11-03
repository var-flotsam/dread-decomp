// Unit 285 of tools/tu_map.py (no class hint): whether the object is active: only while its value at
// +0x114 is positive, by asking fn_71001932dc about the object at +0x78 of the one at +0x108 with
// that value; the answer is kept at +0xfd and returned. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit285Target_b46a1;

struct UnkUnit285Link_b46a1 {
    u8 pad_0x0[0x78];
    UnkUnit285Target_b46a1* target;
};

struct UnkUnit285_b46a1 {
    u8 pad_0x0[0xfd];
    bool active;
    u8 pad_0xfe[0x108 - 0xfe];
    UnkUnit285Link_b46a1* link;
    u8 pad_0x110[4];
    f32 value;
};

bool fn_71001932dc(UnkUnit285Target_b46a1* target, f32 value);

bool fn_71002084b4(UnkUnit285_b46a1* self) {
    f32 value = self->value;
    bool active = value > 0.0f ? fn_71001932dc(self->link->target, value) : false;
    self->active = active;
    return active;
}
