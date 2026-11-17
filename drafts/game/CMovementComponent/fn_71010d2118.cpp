// CMovementComponent (unit 2688): adds this frame's root motion, the drive along the component's
// direction and the decaying push to two accumulated displacements. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkContactShape_ovb0048 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkContact_ovb0048 {
    UnkContactShape_ovb0048* shape;
    u8 pad_0x8[0x18];
    f32 normal_x, normal_y, normal_z;  // 0x20
};

struct UnkRootMotionOwner_l0d2118 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkRootMotion_l0d2118 {
    u8 pad_0x0[0x30];
    UnkRootMotionOwner_l0d2118* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkVec3f direction;  // 0x40
    u8 pad_0x4c[0x7c - 0x4c];
    f32 speed;  // 0x7c
    u8 pad_0x80[0xf9 - 0x80];
    bool airborne;  // 0xf9
    u8 pad_0xfa[0x128 - 0xfa];
    UnkContact_ovb0048 contact;  // 0x128
    u8 pad_0x154[0x214 - 0x154];
    UnkVec3f push;      // 0x214
    UnkVec3f damping;   // 0x220
    f32 push_time;      // 0x22c
};

// A transform with a validity flag: bit 1 of `flags` says it holds root motion.
struct UnkRootMotionXform_l0d2118 {
    UnkMatrix44 matrix;
    u8 pad_0x40[0x1c];
    u8 flags;  // 0x5c
};

UnkVec3f fn_71010d23c4(UnkRootMotion_l0d2118* self, const UnkVec3f& side, const UnkVec3f& motion);
f32 fn_71010d0c64(UnkRootMotion_l0d2118* self);

static inline UnkVec3f Cross(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    UnkVec3f r;
    r.x = ay * bz - az * by;
    r.y = az * bx - ax * bz;
    r.z = ax * by - ay * bx;
    return r;
}

// The sideways direction: across the floor's normal when standing on a shape, else across up.
static inline UnkVec3f Side(bool flag, const UnkContact_ovb0048* contact) {
    f32 x = 0.0f, y = 1.0f, z = 0.0f;
    if (contact && !flag && contact->shape && contact->shape->field_0x10) {
        x = -contact->normal_x;
        y = -contact->normal_y;
        z = -contact->normal_z;
    }
    return Cross(x, y, z, 0.0f, 0.0f, 1.0f);
}

// The root motion's translation carried into the frame of `xform`.
static inline UnkVec3f Carried(const UnkMatrix44* motion, const UnkMatrix44* xform) {
    UnkVec3f translation(motion->m[3], motion->m[7], motion->m[11]);
    return fn_7100077c04(fn_710002614c(xform), translation);
}

void fn_71010d2118(UnkRootMotion_l0d2118* self, f32 dt, const UnkRootMotionXform_l0d2118* root,
                   const UnkMatrix44* xform, UnkVec3f* out_moved, UnkVec3f* out_turned) {
    const UnkMatrix44* motion = nullptr;
    if (root->flags & 2)
        motion = &fn_710002614c(&root->matrix);
    if (!self->owner->animation)
        return;

    UnkVec3f moved(0.0f, 0.0f, 0.0f);
    UnkVec3f turned(0.0f, 0.0f, 0.0f);
    if (motion) {
        moved = Carried(motion, xform);
        turned = fn_71010d23c4(self, Side(self->airborne, &self->contact), moved);
    }

    f32 drive = fn_71010d0c64(self);
    if (drive > 0.0f) {
        f32 dx = drive * (self->speed * self->direction.x) * dt;
        f32 dy = drive * (self->speed * self->direction.y) * dt;
        f32 dz = drive * (self->speed * self->direction.z) * dt;
        moved.x = dx + moved.x;
        moved.y = dy + moved.y;
        moved.z = dz + moved.z;
        turned.x = turned.x + dx;
        turned.y = turned.y + dy;
        turned.z = turned.z + dz;
    }

    f32 time = self->push_time;
    if (time > 1.1920929e-07f) {
        f32 step = time > dt ? dt : time;
        f32 px = self->push.x * step;
        f32 py = self->push.y * step;
        f32 pz = step * self->push.z;
        moved.x = px + moved.x * self->damping.x;
        moved.y = py + moved.y * self->damping.y;
        moved.z = pz + moved.z * self->damping.z;
        turned.x = px + turned.x * self->damping.x;
        turned.y = py + turned.y * self->damping.y;
        turned.z = pz + turned.z * self->damping.z;
        self->push_time = time - dt;
    }

    out_moved->x = moved.x + out_moved->x;
    out_moved->y = moved.y + out_moved->y;
    out_moved->z = moved.z + out_moved->z;
    out_turned->x = turned.x + out_turned->x;
    out_turned->y = turned.y + out_turned->y;
    out_turned->z = turned.z + out_turned->z;
}
