// CQuetzoaMultiTargetProjectileMovementComponent unit (tools/tu_map.py unit 964): one step of a
// projectile along its path of points (on the plane z = 0): steers the velocity back onto the
// current segment and towards its end point, keeping its speed, moves on to the next point when
// the segment is passed, then advances the position. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::math::CVector3D;

struct UnkVec3_cs {
    f32 x, y, z;
};

struct UnkQuetzoaPathParams_l641b5c {
    u8 pad_0x0[0x148];
    f32 gain;   // 0x148
    f32 range;  // 0x14c
};

struct UnkQuetzoaPathMovement_l641b5c {
    u8 pad_0x0[0x5d0];
    UnkQuetzoaPathParams_l641b5c* params;  // 0x5d0
    u8 pad_0x5d8[0x18];
    u8 points[0x8];   // 0x5f0, the path's points
    u32 point_count;  // 0x5f8
};

// A point of the path, on the plane.
static inline CVector3D OnPlane(const UnkVec3_cs& p) {
    return CVector3D{p.x, p.y, 0.0f};
}

static inline CVector3D OnPlane(const CVector3D& v) {
    return CVector3D{v.x, v.y, 0.0f};
}

static inline bool Near(f32 value, f32 target) {
    return value + 0.001f >= target && value - 0.001f <= target;
}

// Turns from `from` towards `to` by `weight` (all the way from 1 up).
static inline CVector3D Blend(const CVector3D& from, const CVector3D& to, f32 weight) {
    if (weight >= 1.0f)
        return to;
    return from * (1.0f - weight) + to * weight;
}

void fn_7100641b5c(UnkQuetzoaPathMovement_l641b5c* self, f32 dt, CVector3D* pos, CVector3D* vel, u32* index) {
    if (*index < self->point_count) {
        CVector3D next = OnPlane(*pos + *vel * dt);
        void* points = self->points;
        f32 speed = base::math::Length(*vel);
        CVector3D heading = *vel;
        UnkVec3_cs first = *fn_71000cc374(points, *index - 1);
        UnkVec3_cs second = *fn_71000cc374(points, *index);
        if (Near(first.x, second.x) && Near(first.y, second.y)) {
            ++*index;
            return;
        }

        CVector3D from = OnPlane(first);
        CVector3D to = OnPlane(second);
        CVector3D segment = to - from;
        f32 length_sq = base::math::LengthSq(segment);
        f32 length = sqrtf(length_sq);
        f32 t = base::math::Dot(next - from, segment) / length_sq;
        if (t >= 1.0f)
            ++*index;

        CVector3D dir;
        if (t >= 0.0f && *index < self->point_count) {
            CVector3D to_path = from + segment * t - next;
            UnkQuetzoaPathParams_l641b5c* params = self->params;
            CVector3D corrected = next + base::math::Normalized(to_path) * dt *
                                             (base::math::Length(to_path) / params->range) * params->gain;
            CVector3D away = base::math::Normalized(corrected - *pos);
            CVector3D ahead = OnPlane(*fn_71000cc374(points, *index)) - OnPlane(corrected);
            dir = Blend(away, base::math::Normalized(ahead), self->params->gain * dt);
        } else {
            CVector3D along = segment * (1.0f / (length + 0.0001f));
            dir = Blend(heading * (1.0f / (speed + 0.0001f)), along, dt * 10.0f);
        }
        f32 new_speed = base::math::Length(*vel);
        *vel = base::math::Normalized(dir) * new_speed;
    }
    *pos = *pos + *vel * dt;
}
