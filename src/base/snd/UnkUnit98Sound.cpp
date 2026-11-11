// Unit 98 (0x71000f35d0-0x71000f43c0, no class hint): a sound controller over two nn::atk players
// (+0x30 and +0x38; the flag at +0x78 picks the first) and a sound handle (+0x28), active while the
// flag at +0x72 is set. Its setters clamp and forward a value (fn_71000f3d4c, fn_71000f3d7c,
// fn_71000f3db8) or switch players (fn_71000f3df4). Names and layouts are placeholders.
#include "base/math/UnkSimd.h"
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkPlayer98 {
    u8 pad_0x0[0xb4];
    f32 field_0xb4;
    u8 pad_0xb8[4];
    f32 field_0xbc;
};

struct UnkSoundControl98 {
    u32 field_0x0;
    u32 field_0x4;
    f32 field_0x8;  // the fade's target (0 when switching on, 1 when off)
    f32 field_0xc;  // the fade's time
    u32 field_0x10;
    u8 pad_0x14[0x14];
    void** field_0x28;  // the sound handle
    UnkAtkPlayer98* field_0x30;
    UnkAtkPlayer98* field_0x38;
    u8 pad_0x40[0x32];
    bool field_0x72;  // active
    u8 pad_0x73;
    bool field_0x74;  // switched on
    bool field_0x75;  // locked
    u8 pad_0x76[2];
    bool field_0x78;  // the first player is in use

    UnkAtkPlayer98* Player() const { return field_0x78 ? field_0x30 : field_0x38; }
};
static_assert(__builtin_offsetof(UnkSoundControl98, field_0x78) == 0x78, "layout");

// nn::atk (as declared in src/base/snd/CSoundSystemATK.cpp).
void fn_710120e5d0(void* sound, f32 value, u32 x1);
void fn_71012066dc(UnkAtkPlayer98* player, u32 value);

void fn_71000f3d4c(UnkSoundControl98* self, f32 value) {
    if (!self->field_0x72)
        return;
    void** handle = self->field_0x28;
    f32 v = __builtin_isnan(value) ? 0.0f : value;
    if (*handle)
        fn_710120e5d0(*handle, v < 0.0f ? 0.0f : v, 0);
}

void fn_71000f3d7c(UnkSoundControl98* self, f32 value) {
    if (!self->field_0x72)
        return;
    UnkAtkPlayer98* player = self->Player();
    player->field_0xb4 = value <= 0.0f ? 0.0f : (value >= 2.0f ? 2.0f : value);
}

void fn_71000f3db8(UnkSoundControl98* self, f32 value) {
    if (!self->field_0x72)
        return;
    UnkAtkPlayer98* player = self->Player();
    player->field_0xbc = value <= -2.0f ? -2.0f : (value >= 2.0f ? 2.0f : value);
}

void fn_71000f3df4(UnkSoundControl98* self, bool first) {
    if (!self->field_0x72)
        return;
    if (first)
        fn_71012066dc(self->field_0x30, 0);
    self->field_0x78 = first;
}

// The frame source (its frame rate at +0x40), as src/base/input/UnkVibrationThread.cpp declares it.
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

static inline s32 Frames(f32 time) {
    f32 rate = *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g_unk_7101d47328) + 0x40);
    return (s32)(time / (1.0f / rate));
}

// Switches on or off with a fade of `time` seconds (unless locked or already so).
void fn_71000f3e30(UnkSoundControl98* self, bool on, f32 time) {
    if (!self->field_0x72 || self->field_0x75 || self->field_0x74 == on)
        return;
    f32 target = on ? 0.0f : 1.0f;
    self->field_0x4 = self->field_0x0;
    self->field_0x74 = on;
    self->field_0x10 = 0;
    self->field_0x8 = target;
    self->field_0xc = time;
    if (self->field_0x78)
        fn_7101206678(self->field_0x30, on, Frames(time));
    else
        fn_7101206678(self->field_0x38, on, Frames(time));
}

// nn::atk: a 3D position (in a 4-lane vector with w = 0) for the player.
void fn_71012053d4(UnkAtkPlayer98* player, const base::math::simd::Vec4& position);

// Passes `position` scaled from centimetres to metres to the first player, when it is in use
// (fn_71000f3c7c and its identical twin fn_71000f3ce4).
static inline void SetPosition(UnkSoundControl98* self, const f32* position) {
    if (!self->field_0x72 || !self->field_0x78)
        return;
    fn_71012053d4(self->field_0x30,
                  base::math::simd::Vec4{position[0] * 0.01f, position[1] * 0.01f, position[2] * 0.01f, 0.0f});
}

void fn_71000f3c7c(UnkSoundControl98* self, const f32* position) { SetPosition(self, position); }
void fn_71000f3ce4(UnkSoundControl98* self, const f32* position) { SetPosition(self, position); }
