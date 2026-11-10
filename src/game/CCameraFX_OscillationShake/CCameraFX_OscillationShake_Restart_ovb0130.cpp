// CCameraFX_OscillationShake's unit (tools/tu_map.py, unit 1170). Names and layouts are placeholders.
#include "types.h"

extern "C" int rand();

// As src/game/CCameraFX_RandShake/CCameraFX_RandShake_Offset_ovb0131.cpp declares it.
struct UnkRandShake_ovb0131;
void fn_71010f3e94(UnkRandShake_ovb0131*);

struct UnkShakeRange_ovb0130 {
    f32 base;
    f32 spread;
};

struct UnkOscillationShake_ovb0130 {
    u8 pad_0x0[0x60];
    UnkShakeRange_ovb0130 ranges[3];  // 0x60
    u8 pad_0x78[0x4];
    f32 base[3];       // 0x7c
    f32 spread[3];     // 0x88
    f32 offset[3];     // 0x94
    f32 direction[3];  // 0xa0
    f32 phase[3];      // 0xac
};
static_assert(__builtin_offsetof(UnkOscillationShake_ovb0130, phase) == 0xac, "layout");

static inline f32 RandRange_ovb0130(f32 lo, f32 hi) {
    f32 t = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return lo + (hi - lo) * t;
}

static inline f32 RandSign_ovb0130() { return (rand() & 1) ? 1.0f : -1.0f; }

// fn_7100767a50: restarts the shake after the base (fn_71010f3e94), as the model updater's wobble
// (fn_7101172de0, src/game/CModelUpdaterComponent/CModelUpdaterComponent_Model_ovb0032.cpp) does.
void fn_7100767a50(UnkOscillationShake_ovb0130* self) {
    fn_71010f3e94(reinterpret_cast<UnkRandShake_ovb0131*>(self));
    for (int i = 0; i < 3; ++i) {
        self->base[i] = self->ranges[i].base;
        self->spread[i] = self->ranges[i].spread;
    }
    for (int i = 0; i < 3; ++i) {
        f32 v = self->base[i];
        self->offset[i] = RandRange_ovb0130(v - v * 0.5f, v + v * 0.5f);
        self->phase[i] = 0.0f;
        self->direction[i] = RandSign_ovb0130();
    }
}
