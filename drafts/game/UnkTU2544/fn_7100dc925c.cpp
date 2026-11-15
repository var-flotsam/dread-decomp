// CBossRushSurvivalConfiguration's unit (unit 2544): the time left of a running timer (+0xbc) after
// taking off the stage count (+0xa50), eased in by the ramp timer (+0xa5c). Names and layouts are
// placeholders.
#include "types.h"

extern f32 g_unk_7101aa94a4;
extern f32 g_unk_7101aa94a8;
extern f32 g_unk_7101aa949c;
extern f32 g_unk_7101aa94a0;

struct UnkSurvivalConfiguration_dc925c {
    u8 pad_0x0[0xbb];
    bool running;  // 0xbb
    f32 time;      // 0xbc
    u8 pad_0xc0[0x990];
    s32 count;     // 0xa50
    f32 timer1;    // 0xa54
    f32 timer2;    // 0xa58
    f32 ramp;      // 0xa5c
};
static_assert(__builtin_offsetof(UnkSurvivalConfiguration_dc925c, ramp) == 0xa5c, "layout");

f32 fn_7100dc925c(UnkSurvivalConfiguration_dc925c* self) {
    if (!self->running)
        return 0.0f;

    f32 time = self->time;
    s32 count = self->count;
    if (count < 1)
        return time;
    if (self->timer1 < g_unk_7101aa94a4 || self->timer2 < g_unk_7101aa94a8 + g_unk_7101aa949c) {
        time -= (f32)count;
        if (time < 0.0f)
            return 0.0f;
        return time;
    }
    f32 ramp = self->ramp;
    if (ramp < g_unk_7101aa94a0) {
        time -= (1.0f - ramp / g_unk_7101aa94a0) * (f32)count;
        return !(time < 0.0f) ? time : 0.0f;
    }
    return time;
}
