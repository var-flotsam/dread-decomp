// CPhotoModeCameraCtrl's free-fly update (tools/tu_map.py unit 1490): restores the saved pose when asked,
// moves the camera along its right/up/forward axes by the input scaled with the frame time (clamping the
// height), then turns the view direction with the two stick axes, keeping the old direction when the
// turn would flip it. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU0.h"
#include "game/UnkTU54.h"
#include "types.h"

struct UnkGfxFrameSource {
    u8 pad_0x0[0x30];
    f32 dt;  // 0x30
};

char* fn_71010ede94(char* p0);
f32 fn_71010ee560(void* self);
UnkVec3f fn_710007a320(const UnkVec3f* v, f32 angle);

struct UnkPhotoModeUpdate_l9cb740 {
    u8 pad_0x0[0x38];
    UnkVec3f home;      // 0x38
    UnkVec3f home_dir;  // 0x44
    u8 pad_0x50[0x18];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0x14];
    f32 field_0x94;
    u8 pad_0x98[0xb18];
    bool fast;       // 0xbb0
    bool slow;       // 0xbb1
    bool back;       // 0xbb2
    bool forward;    // 0xbb3
    f32 side;        // 0xbb4
    f32 rise;        // 0xbb8
    f32 turn_y;      // 0xbbc
    f32 turn_x;      // 0xbc0
    bool restore;    // 0xbc4
    UnkVec3f saved_pos;  // 0xbc8
    UnkVec3f saved_dir;  // 0xbd4
};

static inline UnkVec3f Unit(f32 x, f32 y, f32 z) {
    f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
    return UnkVec3f(x * inv, y * inv, z * inv);
}

void fn_71009cb740(UnkPhotoModeUpdate_l9cb740* self) {
    fn_71010ede94(reinterpret_cast<char*>(self));
    if (self->restore) {
        f32 a = self->saved_pos.x;
        f32 b = self->saved_pos.y;
        self->pos.x = a;
        self->pos.y = b;
        f32 c = self->saved_pos.z;
        self->home.x = a;
        self->home.y = b;
        f32 d = self->saved_dir.x;
        f32 e = self->saved_dir.y;
        self->pos.z = c;
        self->dir.x = d;
        f32 f = self->saved_dir.z;
        self->dir.y = e;
        self->dir.z = f;
        self->home.z = c;
        self->home_dir.x = d;
        self->home_dir.y = e;
        self->home_dir.z = f;
    }
    f32 dt = g_unk_7101d47328->dt * 120.0f;
    f32 turn_speed;
    f32 speed = dt * 0.5f;
    if (self->fast) {
        turn_speed = dt * 5.0f;
        speed = dt * 3.0f;
    } else {
        turn_speed = self->slow ? speed : dt;
        speed = turn_speed;
    }
    f32 lo = dt * 2664.0f;
    f32 hi = dt * 24000.0f;
    f32 v = speed * 8000.0f;
    f32 clamped = v >= hi ? hi : v;
    f32 step = v <= lo ? lo : clamped;

    UnkMatrix44 rot;
    UnkVec3f rotated;
    UnkVec3f dir = self->dir;
    UnkVec3f right = Unit(dir.y * 0.0f - dir.z, dir.z * 0.0f - dir.x * 0.0f, dir.x - dir.y * 0.0f);
    UnkVec3f up = Unit(right.y * dir.z - right.z * dir.y, right.z * dir.x - dir.z * right.x,
                       dir.y * right.x - right.y * dir.x);
    f32 mx = self->side * right.x + 0.0f + up.x * self->rise;
    f32 my = self->side * right.y + 0.0f + up.y * self->rise;
    f32 mz = up.z * self->rise + self->side * right.z + 0.0f;
    if (self->back) {
        mx -= self->dir.x;
        my -= self->dir.y;
        mz -= self->dir.z;
    }
    if (self->forward) {
        mx += self->dir.x;
        my += self->dir.y;
        mz += self->dir.z;
    }
    UnkVec3f m = Unit(mx, my, mz);
    f32 ox = self->pos.x, oy = self->pos.y, oz = self->pos.z;
    f32 nz = step * m.z * 0.0025f + self->home.z;
    self->pos.x = self->home.x + step * m.x * 0.0025f;
    self->pos.y = self->home.y + step * m.y * 0.0025f;
    self->pos.z = nz;
    if (!(nz >= 150.0f && nz <= 5260.0f)) {
        self->pos.x = ox;
        self->pos.y = oy;
        self->pos.z = oz;
    }
    self->field_0x94 = fn_71010ee560(self);

    UnkVec3f old = self->dir;
    if (fabsf(self->turn_y) > 0.3f) {
        f32 angle = turn_speed * (self->turn_y > 0.0f ? -0.0025f : 0.0025f);
        self->dir = fn_710007a320(&self->dir, angle);
        f32 h = sqrtf(old.x * old.x + 0.0f + old.z * old.z);
        UnkVec3f n = Unit(self->dir.x, 0.0f, self->dir.z);
        f32 a = fabsf(n.x * 0.0f + n.y * 0.0f - n.z);
        if (a < 0.5f) {
            f32 inv = 1.0f / (h + 0.0001f);
            f32 b = fabsf(inv * old.x * 0.0f + inv * 0.0f * 0.0f - inv * old.z);
            if (a < b)
                self->dir = old;
        }
    }
    if (fabsf(self->turn_x) > 0.3f) {
        f32 angle = turn_speed * (self->turn_x > 0.0f ? -0.0025f : 0.0025f);
        rot = fn_710007966c(&right, angle);
        rotated = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&rot), &self->dir);
        self->dir = rotated;
    }
    UnkVec3f d = Unit(self->dir.x, self->dir.y, self->dir.z);
    self->dir = d;
}
