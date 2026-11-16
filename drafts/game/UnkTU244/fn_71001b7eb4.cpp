// Unit 244: recomputes a view's up and right axes from its direction: the up axis (a given one, or
// world up), rolled about the direction, then made orthogonal to it. Names and layouts are
// placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkLookAtView;

// The matrix rotating about `axis` by `angle`.
UnkMatrix44 fn_710007966c(const void* value, f32 angle);

struct UnkViewAxes_l1b7eb4 {
    u8 pad_0x0[0x90];
    f32 roll;  // 0x90
    u8 pad_0x94[0x20];
    UnkVec3f dir;    // 0xb4, unit length
    UnkVec3f up;     // 0xc0
    UnkVec3f right;  // 0xcc
    u8 pad_0xd8[0x24];
    f32 aim_roll;  // 0xfc, used while field_0x27e is set
    u8 pad_0x100[0x1c];
    UnkVec3f given_up;  // 0x11c
    u8 pad_0x128[0x156];
    bool field_0x27e;
    u8 pad_0x27f[0x61];
    bool has_given_up;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkViewAxes_l1b7eb4, given_up) == 0x11c, "");
static_assert(__builtin_offsetof(UnkViewAxes_l1b7eb4, field_0x27e) == 0x27e, "");
static_assert(__builtin_offsetof(UnkViewAxes_l1b7eb4, has_given_up) == 0x2e0, "");

void fn_71001b7eb4(UnkLookAtView* view) {
    auto* self = reinterpret_cast<UnkViewAxes_l1b7eb4*>(view);
    f32 ux, uy, uz;
    if (self->has_given_up) {
        self->up = self->given_up;
        ux = self->up.x;
        uy = self->up.y;
        uz = self->up.z;
        // Nearly along the direction: use the z axis instead.
        if (fabsf(self->dir.x * ux + self->dir.y * uy + self->dir.z * uz) > 0.999f) {
            ux = 0.0f;
            uy = 0.0f;
            uz = 1.0f;
            self->up.x = ux;
            self->up.y = uy;
            self->up.z = uz;
        }
    } else {
        ux = 0.0f;
        uy = 1.0f;
        uz = 0.0f;
        self->up.x = ux;
        self->up.y = uy;
        self->up.z = uz;
    }

    f32 roll = self->field_0x27e ? self->aim_roll : self->roll;
    if (fabsf(roll) > 0.001f) {
        UnkMatrix44 rotation = fn_710007966c(&self->dir, -roll);
        self->up = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&rotation), &self->up);
        ux = self->up.x;
        uy = self->up.y;
        uz = self->up.z;
    }

    // right = dir x up, normalised.
    f32 rx = self->dir.y * uz - self->dir.z * uy;
    f32 ry = self->dir.z * ux - uz * self->dir.x;
    f32 rz = uy * self->dir.x - self->dir.y * ux;
    self->right.x = rx;
    self->right.y = ry;
    self->right.z = rz;
    f32 inv = 1.0f / (sqrtf(rz * rz + (rx * rx + ry * ry)) + 0.0001f);
    rx = self->right.x * inv;
    ry = self->right.y * inv;
    rz = inv * self->right.z;
    self->right.x = rx;
    self->right.y = ry;
    self->right.z = rz;

    // up = right x dir.
    self->up.x = ry * self->dir.z - rz * self->dir.y;
    self->up.y = rz * self->dir.x - self->dir.z * rx;
    self->up.z = self->dir.y * rx - ry * self->dir.x;
}
