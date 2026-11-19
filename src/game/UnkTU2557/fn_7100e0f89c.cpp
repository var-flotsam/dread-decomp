// Unit 2557 (tools/tu_map.py): the loading screen's constructor (fn_7100e0f89c): an array of 17 entries
// of 0x18 bytes, a few flags and a block of seven words, all cleared. Names and layouts are placeholders.
#include "game/UnkTU2557.h"
#include "types.h"

struct UnkLoadEntry_e0f89c {
    u64 field_0x0;
    u64 field_0x8;
    s32 field_0x10;
    s32 pad_0x14;
};

// Seven words cleared together.
struct UnkLoadWords_e0f89c {
    u32 words[7];
};

struct UnkLoadingScreen_e0f89c {
    s32 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    UnkLoadEntry_e0f89c entries[17];  // 0x18
    u64 field_0x1b0;
    u64 field_0x1b8;
    f32 field_0x1c0;
    f32 field_0x1c4;
    u8 pad_0x1c8[0x20];
    u8 field_0x1e8;
    u8 pad_0x1e9[3];
    u8 field_0x1ec;
    u8 pad_0x1ed[3];
    u64 field_0x1f0;
    u8 field_0x1f8;
    u8 pad_0x1f9[3];
    f32 field_0x1fc;
    u8 field_0x200;
    u8 pad_0x201[3];
    UnkLoadWords_e0f89c field_0x204;
};
static_assert(sizeof(UnkLoadingScreen_e0f89c) == 0x220, "layout");

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100e0f89c(UnkLoadingScreen* screen) {
    UnkLoadingScreen_e0f89c* self = reinterpret_cast<UnkLoadingScreen_e0f89c*>(screen);
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    for (int i = 0; i < 17; i++) {
        self->entries[i].field_0x0 = 0;
        self->entries[i].field_0x8 = 0;
        self->entries[i].field_0x10 = 0;
    }
    self->entries[0].field_0x10 = 0x50;
    self->field_0x1b0 = 0;
    self->field_0x1b8 = 0;
    self->field_0x1c0 = 0.1f;
    self->field_0x1c4 = 0.1f;
    self->field_0x1e8 = 0;
    self->field_0x1ec = 0;
    self->field_0x1f0 = 0;
    self->field_0x1f8 = 0;
    self->field_0x1fc = -1.0f;
    self->field_0x200 = 0;
    self->field_0x204 = UnkLoadWords_e0f89c();
}
