// In the CSamusMagnetGloveMovementState::CParams unit: computes the angles from the actor to both ends of the
// magnet surface (+0x198, +0x19c), mirrored when facing left and snapped to the axes when close enough.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern UnkTunable g_unk_717273c650;

struct UnkMagnetSnapTunables_l9ac780 {
    u8 pad_0x0[0x22];
    bool snap_floor;        // 0x22
    f32 snap_floor_degrees;  // 0x24
    bool snap_wall;         // 0x28
    f32 snap_wall_degrees;   // 0x2c
};
static_assert(__builtin_offsetof(UnkMagnetSnapTunables_l9ac780, snap_wall_degrees) == 0x2c, "layout");

struct UnkMagnetActor_l9ac780 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;
};

struct UnkMagnetMovement_l9ac780 {
    u8 pad_0x0[0x58];
    CVector3D dir;  // 0x58
};

struct UnkMagnetAngles_l9ac780 {
    u8 pad_0x0[0x30];
    UnkMagnetActor_l9ac780* actor;        // 0x30
    UnkMagnetMovement_l9ac780* movement;  // 0x38
    u8 pad_0x40[0x158];
    f32 start_angle;  // 0x198
    f32 end_angle;    // 0x19c
    bool field_0x1a0;
    u8 pad_0x1a1[0x1f];
    u32 kind;  // 0x1c0
    u8 pad_0x1c4[0xc];
    UnkSegment2D segment;  // 0x1d0
};
static_assert(__builtin_offsetof(UnkMagnetAngles_l9ac780, segment) == 0x1d0, "layout");

static constexpr f32 kPi = 3.14159265f;
static constexpr f32 kTwoPi = 6.28318531f;
static constexpr f32 kHalfPi = 1.57079633f;
static constexpr f32 kThreeHalfPi = 4.71238898f;
static constexpr f32 kDegToRad = 0.0174532925f;

static inline f32 Snapped(f32 angle, f32 target, f32 tolerance) {
    return angle + tolerance >= target && angle - tolerance <= target ? target : angle;
}

static inline void SnapAngles(UnkMagnetAngles_l9ac780* self, f32 start_target, f32 end_target, f32 tolerance) {
    self->start_angle = Snapped(self->start_angle, start_target, tolerance);
    self->end_angle = Snapped(self->end_angle, end_target, tolerance);
}

static inline void ComputeAngles(UnkMagnetAngles_l9ac780* self, const CVector2D& center) {
    const UnkSegment2D& seg = self->segment;
    CVector2D to_start = {seg.start.x - center.x, seg.start.y - center.y};
    CVector2D to_end = {seg.end.x - center.x, seg.end.y - center.y};
    self->start_angle = atan2f(to_start.y, to_start.x);
    self->end_angle = atan2f(to_end.y, to_end.x);
}

static inline void Mirror(UnkMagnetAngles_l9ac780* self) {
    self->start_angle = kPi - self->start_angle;
    self->end_angle = kPi - self->end_angle;
    f32 start = self->start_angle;
    self->start_angle = self->end_angle;
    self->end_angle = start;
}

void fn_71009ac780(UnkMagnetAngles_l9ac780* self) {
    CVector2D center;
    const UnkMatrix44& m = self->actor->xform;
    CVector3D pos = {m.m[3], m.m[7], 0.0f};
    fn_710007b430(&center, &pos);
    const UnkMagnetSnapTunables_l9ac780& tun = reinterpret_cast<const UnkMagnetSnapTunables_l9ac780&>(g_unk_717273c650);
    switch (self->kind) {
    case 1:
    case 2: {
        ComputeAngles(self, center);
        const CVector3D& dir = self->movement->dir;
        if (dir.x * 1.0f + dir.y * 0.0f + dir.z * 0.0f != 1.0f)
            Mirror(self);
        if (tun.snap_floor) {
            SnapAngles(self, 0.0f, kPi, tun.snap_floor_degrees * kDegToRad);
        }
        break;
    }
    case 0: {
        UnkVec2Result normal = fn_7100125e78(&self->segment);
        f32 facing = normal.x * 1.0f + normal.y * 0.0f;
        ComputeAngles(self, center);
        if (facing != 1.0f) {
            self->start_angle += kTwoPi;
            Mirror(self);
        }
        self->end_angle += kTwoPi;
        if (tun.snap_wall) {
            SnapAngles(self, kHalfPi, kThreeHalfPi, tun.snap_wall_degrees * kDegToRad);
        }
        break;
    }
    }
    self->field_0x1a0 = false;
}
