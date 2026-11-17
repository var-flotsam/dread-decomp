// CCharClassLockOnMissileMovement unit: one step of a lock-on missile along its path of points,
// pulled back onto the current segment, weaving sideways and turning towards the next point.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU83.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector3D;

// The movement's tunable values read by the step.
struct UnkLockOnMissileSteering : UnkTunable {
    u8 pad_0x8[0x2c];
    f32 field_0x34;  // homing gain
    f32 field_0x38;  // turn rate
    u8 pad_0x3c[0x8];
    f32 field_0x44;  // correction distance
    f32 field_0x48;  // weave amplitude
    f32 field_0x4c;  // weave speed
};
static_assert(__builtin_offsetof(UnkLockOnMissileSteering, field_0x4c) == 0x4c, "layout");
extern UnkTunable g_unk_7172754868;

__attribute__((visibility("hidden"))) extern const u8 g_unk_710164b870[];

struct UnkLockOnMissileMover;

struct UnkLockOnMissileMoverVtable {
    void* pad_0x0[0x338 / 8];
    void (*vf_0x338)(UnkLockOnMissileMover* self, const CVector3D* pos, const CVector3D& velocity,
                     const u8* name, const base::global::CFilePathStrId& path);
};

struct UnkLockOnMissilePoints {
    u8 pad_0x0[0x8];
    u32 count;
    u8 pad_0xc[0x4];
};

struct UnkLockOnMissileMover {
    UnkLockOnMissileMoverVtable* vtable;
    u8 pad_0x8[0x600];
    bool field_0x608;
    u8 pad_0x609[0x3b];
    CVector3D field_0x644;
    u8 pad_0x650[0x1e0];
    UnkLockOnMissilePoints points;  // 0x830
    u8 pad_0x840[0x28];
    bool field_0x868;
};
static_assert(__builtin_offsetof(UnkLockOnMissileMover, field_0x868) == 0x868, "layout");

inline const CVector3D& PathPoint(UnkLockOnMissileMover* self, u32 index) {
    return *reinterpret_cast<const CVector3D*>(fn_71000cc374(&self->points, index));
}

inline bool IsNear(f32 a, f32 b) {
    return a + 0.001f >= b && a - 0.001f <= b;
}

inline f32 AtLeast(f32 value, f32 low) {
    return !(value < low) ? value : low;
}

// Turns from one direction towards another by a fraction, reaching it at 1.
inline CVector3D Blend(const CVector3D& from, const CVector3D& to, f32 fraction) {
    if (fraction >= 1.0f)
        return to;
    return from * (1.0f - fraction) + to * fraction;
}

void fn_7100ab7774(UnkLockOnMissileMover* self, CVector3D* pos, CVector3D* vel, u32* index, f32* phase,
                   f32 dt) {
    if (*index >= self->points.count) {
        *pos = *vel * dt + *pos;
        self->field_0x868 = true;
        return;
    }

    CVector3D next = *vel * dt + *pos;
    const UnkLockOnMissileSteering& tunable = static_cast<UnkLockOnMissileSteering&>(g_unk_7172754868);
    f32 homing = tunable.field_0x34;
    f32 turnRate = tunable.field_0x38;
    f32 correction = tunable.field_0x44;
    f32 weaveAmplitude = tunable.field_0x48;
    f32 weaveSpeed = tunable.field_0x4c;

    CVector3D last = PathPoint(self, self->points.count - 1);
    CVector3D target = self->field_0x644;
    CVector3D velDir = Normalized(*vel);
    f32 speed = Length(*vel);

    CVector3D from = PathPoint(self, *index - 1);
    CVector3D to = PathPoint(self, *index);
    if (IsNear(from.x, to.x) && IsNear(from.y, to.y) && IsNear(from.z, to.z)) {
        ++*index;
        return;
    }

    CVector3D flat = {next.x, next.y, 0.0f};
    CVector3D seg = to - from;
    CVector3D segDir = Normalized(seg);
    f32 t = Dot(seg, flat - from) / LengthSq(seg);
    f32 gain = AtLeast(homing, 1.0f) * 0.01f;
    if (t >= 1.0f)
        ++*index;

    CVector3D dir;
    CVector3D end;
    if (t >= 0.0f && *index < self->points.count) {
        CVector3D off = (from + seg * t) - flat;
        CVector3D offDir = Normalized(off);
        f32 dist = Length(off);
        speed = self->field_0x608 ? speed * 0.5f : speed;

        CVector3D step;
        if (self->field_0x608)
            step = off * gain * dt;
        else
            step = offDir * dt * (dist / AtLeast(correction, 1.0f)) * gain;
        next = step + next;

        CVector3D delta = next - *pos;
        if (!self->field_0x608 && IsNear(LengthSq(delta), 0.0f)) {
            self->vtable->vf_0x338(self, &next, g_unk_7101647790, g_unk_710164b870, g_unk_7101d470e8);
            return;
        }

        f32 ratio = DistanceSq(last, flat) / DistanceSq(last, target);
        f32 turn = AtLeast(turnRate, 1.0f) * 0.01f;
        f32 amplitude = AtLeast(weaveAmplitude, 1.0f) * 0.01f;
        CVector3D deltaDir = Normalized(delta);
        CVector3D up = {0.0f, 0.0f, 1.0f};
        CVector3D side = Cross(up, deltaDir);
        CVector3D weave = segDir + side * sinf(*phase) * amplitude * ratio;
        CVector3D turned = Blend(deltaDir, weave, turn * dt);
        end = next;

        const CVector3D& point = PathPoint(self, *index);
        CVector3D toPoint = {point.x - next.x, point.y - next.y, 0.0f};
        dir = Blend(turned, Normalized(toPoint), gain * dt);
    } else {
        end = next;
        dir = Blend(velDir, segDir, gain * dt);
    }

    *phase = AtLeast(weaveSpeed, 0.0f) * 0.01f * dt + *phase;
    *vel = dir * speed;
    if (self->field_0x608)
        *pos = end;
    else
        *pos = *vel * dt + *pos;
}
