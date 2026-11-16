// CModelUpdaterComponent's unit (tools/tu_map.py unit 2719): advances a timed wobble. While it runs, each of
// three offsets swings back and forth between random limits, and the result is the base position moved
// along the axes around a direction, scaled by a curve over the run (1 without a curve); when the time is
// up, the timer and the result are cleared. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU53.h"
#include "types.h"

extern "C" int rand();

struct UnkWobbleCurve_l172f40 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    u32 use_count;
};

struct UnkWobbleVec_l172f40 {
    f32 x, y, z;
};

static inline UnkWobbleVec_l172f40 Cross(const UnkWobbleVec_l172f40& a, const UnkWobbleVec_l172f40& b) {
    UnkWobbleVec_l172f40 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

struct UnkWobble_l172f40 {
    u8 pad_0x0[0x18];
    UnkWobbleCurve_l172f40* curve;  // 0x18
    UnkWobbleVec_l172f40 direction;  // 0x20
    UnkWobbleVec_l172f40 base;  // 0x2c
    f32 duration;  // 0x38
    f32 time;  // 0x3c
    f32 range[3];  // 0x40
    f32 speed[3];  // 0x4c
    f32 limit[3];  // 0x58
    f32 sign[3];  // 0x64
    UnkWobbleVec_l172f40 offset;  // 0x70
    UnkWobbleVec_l172f40 result;  // 0x7c
};

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = RandUnit();
    return lo + (hi - lo) * r;
}

static inline f32 Clamp01(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

void fn_7101172f40(UnkWobble_l172f40* self, f32 dt) {
    if (!(self->duration > 0.0f))
        return;
    if (!(self->time < self->duration)) {
        self->duration = 0.0f;
        self->time = 0.0f;
        self->result = UnkWobbleVec_l172f40();
        return;
    }
    self->time += dt;

    f32 strength;
    if (self->curve == reinterpret_cast<UnkWobbleCurve_l172f40*>(&g_unk_7101d47108->empty)) {
        strength = 1.0f;
    } else {
        f32 t = self->time / self->duration;
        ++self->curve->use_count;
        const char* text = self->curve->text;
        strength = fn_7100076020(Clamp01(t), text ? text : g_unk_710162e2ed);
    }

    f32* offset = &self->offset.x;
    for (int i = 0; i < 3; i++) {
        f32 sign = self->sign[i];
        offset[i] += self->speed[i] * dt * sign;
        if ((sign < 0.0f && offset[i] <= -self->limit[i]) || (sign > 0.0f && offset[i] >= self->limit[i])) {
            self->sign[i] = -sign;
            f32 half = self->range[i] * 0.5f;
            self->limit[i] = RandRange(self->range[i] - half, self->range[i] + half);
        }
    }

    UnkWobbleVec_l172f40 up = {0.0f, 1.0f, 0.0f};
    UnkWobbleVec_l172f40 side = Cross(self->direction, up);
    UnkWobbleVec_l172f40 normal = Cross(side, self->direction);
    const UnkWobbleVec_l172f40& dir = self->direction;
    self->result.x = self->base.x + side.x * self->offset.x * strength + normal.x * self->offset.y * strength +
                     dir.x * self->offset.z * strength;
    self->result.y = self->base.y + side.y * self->offset.x * strength + normal.y * self->offset.y * strength +
                     dir.y * self->offset.z * strength;
    self->result.z = self->base.z + side.z * self->offset.x * strength + normal.z * self->offset.y * strength +
                     dir.z * self->offset.z * strength;
}
