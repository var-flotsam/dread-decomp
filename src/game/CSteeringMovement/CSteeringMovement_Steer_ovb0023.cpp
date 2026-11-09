// CSteeringMovement's unit (tools/tu_map.py). fn_71010df408 turns the owner's transform (+0x38 of
// the owner at +0x30) to face the movement's direction (+0x40). fn_71010df1fc, the update: after the
// base step (fn_71010d1b60) records the owner's position and angles, adds the steering force
// (vf_0x280) over the mass (+0x230) to the velocity (direction times speed, +0x7c), keeps its
// direction and its length capped at +0x234, then runs vf_0x288 and vf_0x290. Names and layouts are
// placeholders.
#include <math.h>
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSteeringOwner_ovb0023 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x3b0 - 0x94];
    bool field_0x3b0;
    u8 pad_0x3b1[0x3c0 - 0x3b1];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkSteeringOwner_ovb0023, field_0x3c0) == 0x3c0, "layout");

struct UnkSteeringMovement_ovb0023 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkSteeringOwner_ovb0023* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkVec3f dir;  // 0x40
    u8 pad_0x4c[0x7c - 0x4c];
    f32 speed;  // 0x7c
    u8 pad_0x80[0x4];
    UnkVec3f position;  // 0x84
    UnkVec3f angle;     // 0x90
    u8 pad_0x9c[0x230 - 0x9c];
    f32 mass;       // 0x230
    f32 max_speed;  // 0x234
};

void fn_71010df408(UnkSteeringMovement_ovb0023* self) {
    f32 pitch = atan2f(-self->dir.y, sqrtf(self->dir.x * self->dir.x + self->dir.z * self->dir.z));
    f32 yaw = atan2f(self->dir.x, self->dir.z);
    UnkSteeringOwner_ovb0023* owner = self->owner;
    owner->xform.SetAngle(UnkVec3f(pitch, yaw, 0.0f));
    owner->field_0x3c0 = true;
}

struct UnkSteeringMovementV_ovb0023 {
    u8 pad_0x0[0x280];
    UnkVec3f (*vf_0x280)(UnkSteeringMovement_ovb0023* self, f32 dt);
    void (*vf_0x288)(UnkSteeringMovement_ovb0023* self, f32 dt);
    void (*vf_0x290)(UnkSteeringMovement_ovb0023* self, f32 dt);
};

void fn_71010d1b60(UnkSteeringMovement_ovb0023* self, f32 dt);

void fn_71010df1fc(UnkSteeringMovement_ovb0023* self, f32 dt) {
    fn_71010d1b60(self, dt);
    UnkSteeringOwner_ovb0023* owner = self->owner;
    self->position = owner->xform.GetTranslation();
    self->angle = owner->xform.GetAngle();
    const UnkSteeringMovementV_ovb0023* vt = *reinterpret_cast<UnkSteeringMovementV_ovb0023**>(self);
    UnkVec3f force = vt->vf_0x280(self, dt);
    f32 inv = 1.0f / self->mass;
    f32 ax = inv * force.x, ay = inv * force.y, az = inv * force.z;
    f32 vx = self->speed * self->dir.x, vy = self->speed * self->dir.y, vz = self->speed * self->dir.z;
    ax *= dt;
    ay *= dt;
    az *= dt;
    UnkVec3f v;
    v.x = ax + vx;
    v.y = ay + vy;
    v.z = az + vz;
    f32 scale = 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
    self->dir = UnkVec3f(v.x * scale, v.y * scale, v.z * scale);
    f32 len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    self->speed = len > self->max_speed ? self->max_speed : len;
    vt = *reinterpret_cast<UnkSteeringMovementV_ovb0023**>(self);
    vt->vf_0x288(self, dt);
    vt = *reinterpret_cast<UnkSteeringMovementV_ovb0023**>(self);
    vt->vf_0x290(self, dt);
}
