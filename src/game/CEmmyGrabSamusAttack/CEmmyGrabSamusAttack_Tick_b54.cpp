// CEmmyGrabSamusAttack unit (tools/tu_map.py, the name from reflection): two identical updates. Each adds
// the elapsed time to the timer at +0x6b4, lowers the level at +0x6d4 by 0.8 per second (not below zero),
// and restarts the effects (fn_710050f26c) with the timings at +0x6d0 and +0x6cc, the colours (0, 0.8, 0)
// and (0, 1, 0) at that level's alpha, and an offset of 0.2. Names and layouts are placeholders.
#include "types.h"

// The colour types of fn_710050f26c (src/game/CEmmyGrabSamusAttack/fn_710050f26c.cpp), named the same so
// the call resolves to that definition.
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
void fn_710050f26c(UnkStruct_710050f26c* self, f32 time, const UnkColorIn_710050f26c* color, f32 time2,
                   const UnkColor_710050f26c* color2, f32 offset);

struct UnkGrabTick_b54 {
    u8 unk_0x0[0x6b4];
    f32 timer;  // 0x6b4
    u8 unk_0x6b8[0x14];
    f32 field_0x6cc;
    f32 field_0x6d0;
    f32 level;  // 0x6d4
};

template <class T>
static inline __attribute__((always_inline)) const T* Addr_b54(const T& value) {
    return &value;
}

static inline __attribute__((always_inline)) void Tick_b54(UnkGrabTick_b54* self, f32 dt) {
    self->timer += dt;
    f32 level = self->level + dt * -0.8f;
    level = level < 0.0f ? 0.0f : level;
    self->level = level;
    fn_710050f26c(reinterpret_cast<UnkStruct_710050f26c*>(self), self->field_0x6d0,
                  Addr_b54(UnkColorIn_710050f26c{{0.0f, 0.8f, 0.0f}, level}), self->field_0x6cc,
                  Addr_b54(UnkColor_710050f26c{0.0f, 1.0f, 0.0f, level}), 0.2f);
}

void fn_71005110d4(UnkGrabTick_b54* self, f32 dt) {
    Tick_b54(self, dt);
}

void fn_7100511324(UnkGrabTick_b54* self, f32 dt) {
    Tick_b54(self, dt);
}
