// Unit 259: advances two timed screen effects by the frame time. While each runs, it writes its
// strength into the shared effect parameters; when it ends, the parameters go back to their
// defaults. Names and layouts are placeholders.
#include "game/UnkTU53.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

// The effect parameters the renderer reads (0x5c bytes), and their defaults.
struct UnkEffectParams_l1d8000 {
    u8 pad_0x0[0x30];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    u8 pad_0x40[0x4];
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u8 pad_0x50[0xc];
};
static_assert(sizeof(UnkEffectParams_l1d8000) == 0x5c, "UnkEffectParams_l1d8000 layout");

// The two effects' state.
struct UnkEffectTimers_l1d8000 {
    bool first_active;
    bool second_active;
    u8 pad_0x2[0x6];
    f32 first_time;   // 0x8, counts down
    f32 second_time;  // 0xc, counts down
    u8 pad_0x10[0x40];
    f32 first_duration;  // 0x50
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
    f32 field_0x68;
};
static_assert(__builtin_offsetof(UnkEffectTimers_l1d8000, field_0x68) == 0x68, "UnkEffectTimers_l1d8000 layout");

extern HIDDEN UnkEffectTimers_l1d8000 g_unk_716e595f88;
// The parameters, after a scale the second effect reads.
struct UnkEffectState_l1d8000 {
    f32 scale;
    UnkEffectParams_l1d8000 params;  // 0x4
};
extern HIDDEN UnkEffectState_l1d8000 g_unk_7101a1e84c;
extern HIDDEN const UnkEffectParams_l1d8000 g_unk_7101648b48;

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71001d8000(f32 dt) {
    UnkEffectTimers_l1d8000& s = g_unk_716e595f88;
    UnkEffectState_l1d8000& e = g_unk_7101a1e84c;
    UnkEffectParams_l1d8000& p = e.params;
    if (s.first_time > 0.0f) {
        f32 t = s.first_time / s.first_duration;
        f32 rest = 1.0f - t;
        p.field_0x4c = t * s.field_0x64 + rest * 0.001f;
        p.field_0x30 = t * s.field_0x54;
        p.field_0x34 = rest * (p.field_0x38 + 1.0f);
        s.first_active = true;
        s.second_active = true;
        p.field_0x3c = fn_7100076020(t, "cs") * s.field_0x68 + 1.0f;
        s.first_time -= dt;
        if (s.first_time <= 0.0f) {
            s.first_active = false;
            s.second_active = false;
            s.first_time = 0.0f;
            p = g_unk_7101648b48;
        }
    }
    if (s.second_time > 0.0f) {
        f32 t = s.second_time / 0.15f;
        f32 x = t * e.scale;
        f32 y = t * 0.0f;
        s.second_time -= dt;
        s.second_active = true;
        p.field_0x44 = x;
        p.field_0x48 = y;
        if (s.second_time <= 0.0f) {
            s.second_active = false;
            s.second_time = 0.0f;
            p = g_unk_7101648b48;
        }
    }
}
