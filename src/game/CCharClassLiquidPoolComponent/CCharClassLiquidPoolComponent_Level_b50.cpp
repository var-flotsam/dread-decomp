// CCharClassLiquidPoolComponent unit (tools/tu_map.py, the name from reflection): after fn_7100a45ac4,
// moves the level (+0xc50) from its start (+0xc54) towards the target (+0xc58) over the duration at
// +0xc5c, advancing the elapsed time (+0xc60) by `dt`, then applies it (fn_7100900928); nothing happens
// once the level is within 1e-5 of the target. Names and layouts are placeholders.
#include "game/CCharClassLiquidPoolComponent.h"
#include "types.h"

struct UnkLiquidPoolLevel {
    u8 unk_0x0[0xc50];
    f32 level;     // 0xc50
    f32 start;     // 0xc54
    f32 target;    // 0xc58
    f32 duration;  // 0xc5c
    f32 elapsed;   // 0xc60
};

void fn_7100a45ac4(UnkLiquidPoolLevel* self, f32 dt);

void fn_710090087c(UnkLiquidPoolLevel* self, f32 dt) {
    fn_7100a45ac4(self, dt);
    f32 level = self->level;
    f32 target = self->target;
    if (level - 1e-5f <= target && level + 1e-5f >= target)
        return;
    f32 t = self->elapsed + dt;
    self->elapsed = t;
    if (t < self->duration) {
        f32 k = t / self->duration;
        k = 1.0f < k ? 1.0f : k;
        self->level = self->start + k * (target - self->start);
    } else {
        self->level = target;
    }
    fn_7100900928(self);
}
