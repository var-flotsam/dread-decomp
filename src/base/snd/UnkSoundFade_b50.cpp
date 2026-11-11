// Unit 99: the switch with a fade of UnkUnit98Sound.cpp's fn_71000f3e30, for a controller whose flags
// sit at +0x16c (active) and +0x16d (switched on) and which always drives the player at +0x30.
// Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkPlayer99;

struct UnkSoundControl99 {
    u32 field_0x0;
    u32 field_0x4;
    f32 field_0x8;  // the fade's target (0 when switching on, 1 when off)
    f32 field_0xc;  // the fade's time
    u32 field_0x10;
    u8 pad_0x14[0x1c];
    UnkAtkPlayer99* field_0x30;
    u8 pad_0x38[0x134];
    bool field_0x16c;  // active
    bool field_0x16d;  // switched on
};
static_assert(__builtin_offsetof(UnkSoundControl99, field_0x16d) == 0x16d, "layout");

// As src/base/snd/UnkUnit98Sound.cpp declares them.
struct UnkAtkPlayer98;
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

static inline s32 Frames(f32 time) {
    f32 rate = *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g_unk_7101d47328) + 0x40);
    return (s32)(time / (1.0f / rate));
}

void fn_71000f4d30(UnkSoundControl99* self, bool on, f32 time) {
    if (!self->field_0x16c || self->field_0x16d == on)
        return;
    self->field_0x16d = on;
    f32 target = on ? 0.0f : 1.0f;
    self->field_0x4 = self->field_0x0;
    self->field_0x8 = target;
    self->field_0xc = time;
    self->field_0x10 = 0;
    fn_7101206678(reinterpret_cast<UnkAtkPlayer98*>(self->field_0x30), on, Frames(time));
}
