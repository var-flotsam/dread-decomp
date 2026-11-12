// Unit 2557 (tools/tu_map.py): the loading screen singleton's creation (fn_7100e0f660): on first use it is
// allocated (0x220 bytes, flags 0x3f) and constructed in place (the constructor fn_7100e0f89c inlined: an
// array of 17 entries of 0x18 bytes, a few flags and a block of seven words), then the instance is handed
// to fn_7100e0f77c. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU0.h"
#include "types.h"

struct UnkLoadEntry_b0247 {
    u64 field_0x0;
    u64 field_0x8;
    s32 field_0x10;
    s32 pad_0x14;
};

struct UnkLoadingScreen_b0247 {
    s32 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    UnkLoadEntry_b0247 entries[17];  // 0x18
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
    u32 field_0x1fc;
    u8 field_0x200;
    u8 pad_0x201[3];
    u32 field_0x204[7];
};
static_assert(sizeof(UnkLoadingScreen_b0247) == 0x220, "layout");

static inline void InitLoadingScreen(UnkLoadingScreen_b0247* self) {
    self->entries[0].field_0x10 = 0x50;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x1c0 = 0.1f;
    self->field_0x1c4 = 0.1f;
    self->field_0x0 = 0;
    for (int i = 1; i < 17; i++)
        self->entries[i].field_0x10 = 0;
    self->field_0x1e8 = 0;
    self->field_0x1ec = 0;
    self->field_0x1f8 = 0;
    self->field_0x1f0 = 0;
    self->field_0x1fc = 0xbf800000u;
    self->field_0x200 = 0;
    for (int i = 0; i < 17; i++) {
        self->entries[i].field_0x0 = 0;
        self->entries[i].field_0x8 = 0;
    }
    self->field_0x1b0 = 0;
    self->field_0x1b8 = 0;
    self->field_0x204[6] = 0;
    self->field_0x204[4] = 0;
    self->field_0x204[5] = 0;
    self->field_0x204[2] = 0;
    self->field_0x204[3] = 0;
    self->field_0x204[0] = 0;
    self->field_0x204[1] = 0;
}

struct UnkLoadingScreen;  // src/game/UnkLoadingScreen.cpp
void fn_7100e0f77c(UnkLoadingScreen* self);

void fn_7100e0f660() {
    if (!g_unk_7101d47380) {
        UnkLoadingScreen_b0247* p = static_cast<UnkLoadingScreen_b0247*>(fn_71000839a0(0x220, 0x3f));
        InitLoadingScreen(p);
        g_unk_7101d47380 = reinterpret_cast<UnkLoadingScreen*>(p);
    }
    fn_7100e0f77c(g_unk_7101d47380);
}
