// CEmmyGrabSamusAttack's unit (tools/tu_map.py): two more updates of the shape of
// CEmmyGrabSamusAttack_Tick_b54.cpp's (timer +0x6b4, level +0x6d4 lowered by 0.8 per second, effects
// restarted through fn_710050f26c, here with the colours (0.8, 0, 0) and (1, 0, 0)), each followed by a
// countdown at +0x6c0: when not negative it runs down by `dt` and, at zero or below, is parked at -1.
// The second clears +0x6cc first. Names and layouts are placeholders.
#include "game/CEmmyGrabSamusAttack.h"
#include "types.h"

// The colour types of fn_710050f26c, as CEmmyGrabSamusAttack_Tick_b54.cpp names them.
struct UnkRGB_710050f26c {
    f32 r;
    f32 g;
    f32 b;
};
struct UnkColorIn_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
};
struct UnkColor_710050f26c {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

struct UnkStruct_710050f26c;

struct UnkGrabTick_A3_b64 {
    u8 unk_0x0[0x6b4];
    f32 timer;  // 0x6b4
    u8 unk_0x6b8[0x8];
    f32 countdown;  // 0x6c0
    u8 unk_0x6c4[0x8];
    f32 field_0x6cc;
    f32 field_0x6d0;
    f32 level;  // 0x6d4
};

template <class T>
static inline __attribute__((always_inline)) const T* Addr_A3_b64(const T& value) {
    return &value;
}

static inline __attribute__((always_inline)) void Tick_A3_b64(UnkGrabTick_A3_b64* self, f32 dt) {
    self->timer += dt;
    f32 level = self->level + dt * -0.8f;
    level = level < 0.0f ? 0.0f : level;
    self->level = level;
    fn_710050f26c(reinterpret_cast<UnkStruct_710050f26c*>(self), self->field_0x6d0,
                  Addr_A3_b64(UnkColorIn_710050f26c{{0.8f, 0.0f, 0.0f}, level}), self->field_0x6cc,
                  Addr_A3_b64(UnkColor_710050f26c{1.0f, 0.0f, 0.0f, level}), 0.2f);
}

static inline __attribute__((always_inline)) void CountDown_A3_b64(UnkGrabTick_A3_b64* self, f32 dt) {
    f32 t = self->countdown;
    if (t >= 0.0f) {
        t -= dt;
        self->countdown = t;
        if (t <= 0.0f)
            self->countdown = -1.0f;
    }
}

void fn_7100511188(UnkGrabTick_A3_b64* self, f32 dt) {
    Tick_A3_b64(self, dt);
    CountDown_A3_b64(self, dt);
}

void fn_7100511258(UnkGrabTick_A3_b64* self, f32 dt) {
    self->field_0x6cc = 0.0f;
    Tick_A3_b64(self, dt);
    CountDown_A3_b64(self, dt);
}
