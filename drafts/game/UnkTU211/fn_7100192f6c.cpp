// UnkTU211: advances a particle effect's time by `dt`. Looping (or when forced) it runs whole periods
// through its two lists of emitters, then the rest; otherwise it steps them by the time so far. When
// nothing is alive any more it stops looping and rewinds to one period. Names and layouts are
// placeholders.
#include "types.h"

struct UnkEmitter_b0235;
void fn_7100170184(UnkEmitter_b0235* self, f32 dt);

// The emitters of the first list.
struct UnkEmitterA_192f6c {
    u8 pad_0x0[0x94];
    bool alive;  // 0x94
};
void fn_710016e15c(UnkEmitterA_192f6c* self, f32 time);
void fn_710016e2b4(UnkEmitterA_192f6c* self, f32 time);

// The emitters of the second list.
struct UnkEmitterB_192f6c {
    u8 pad_0x0[0x390];
    u32 alive_count;  // 0x390
};
void fn_7100170044(UnkEmitterB_192f6c* self, f32 time);

struct UnkEffect_192f6c {
    u8 pad_0x0[0x20];
    u32 b_count;  // 0x20
    u8 pad_0x24[0x8];
    u32 a_count;  // 0x2c
    u8 pad_0x30[0x8];
    UnkEmitterB_192f6c** b;  // 0x38
    u8 pad_0x40[0x38];
    UnkEmitterA_192f6c** a;  // 0x78
    f32 period;  // 0x80
    f32 time;    // 0x84
    u8 pad_0x88[0x1d0];
    bool use_a;    // 0x258
    bool looping;  // 0x259
    bool hold;     // 0x25a

    bool AnyAlive() const {
        bool alive = false;
        if (use_a && a_count != 0) {
            u32 i = 0;
            do {
                alive = a[i]->alive;
                ++i;
            } while (i < a_count && !alive);
        }
        if (!alive && b_count != 0) {
            u32 i = 0;
            do {
                alive = b[i]->alive_count != 0;
                ++i;
            } while (i < b_count && !alive);
        }
        return alive;
    }
};
static_assert(__builtin_offsetof(UnkEffect_192f6c, use_a) == 0x258, "UnkEffect_192f6c layout");

void fn_7100192f6c(char* model, f32 dt, bool force) {
    UnkEffect_192f6c* self = reinterpret_cast<UnkEffect_192f6c*>(model);
    f32 period = self->period;
    f32 time = self->time + dt;
    self->time = time;
    if (!self->looping && !force) {
        if (self->use_a) {
            for (u32 i = 0; i < self->a_count; ++i)
                fn_710016e15c(self->a[i], self->time);
        }
        for (u32 i = 0; i < self->b_count; ++i)
            fn_7100170184(reinterpret_cast<UnkEmitter_b0235*>(self->b[i]), self->time);
        self->time = 0.0f;
    } else {
        if (self->use_a) {
            while (time > period) {
                time -= period;
                for (u32 i = 0; i < self->a_count; ++i)
                    fn_710016e2b4(self->a[i], period);
            }
        }
        while (self->time > period) {
            self->time -= period;
            for (u32 i = 0; i < self->b_count; ++i)
                fn_7100170044(self->b[i], period);
        }
        if (!self->hold) {
            if (self->use_a) {
                for (u32 i = 0; i < self->a_count; ++i)
                    fn_710016e2b4(self->a[i], self->time);
            }
            for (u32 i = 0; i < self->b_count; ++i)
                fn_7100170044(self->b[i], self->time);
            self->time = 0.0f;
        }
    }
    if (!self->AnyAlive()) {
        self->looping = false;
        self->time = self->period;
    }
}
