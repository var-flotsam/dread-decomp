// Unit UnkTU2566 (tunable minimap manager): computes the 2D bounds of the area the camera sees,
// as {min x, min y, max x, max y}. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkViewParams {
    u8 pad_0x0[0x80];
    f32 field_0x80;
    u8 pad_0x84[0xb4 - 0x84];
    f32 field_0xb4;
    f32 field_0xb8;
};

struct UnkViewCamera {
    u8 pad_0x0[0x20];
    UnkXformNode xform;
    UnkViewParams* field_0x130;
};
static_assert(__builtin_offsetof(UnkViewCamera, field_0x130) == 0x130, "UnkViewCamera layout");

struct UnkViewOwner {
    u8 pad_0x0[0x40];
    UnkViewCamera* field_0x40;
};

struct UnkMinimapRect {
    f32 min_x, min_y, max_x, max_y;
    UnkMinimapRect() {}
    UnkMinimapRect(const UnkMinimapRect& o) : min_x(o.min_x), min_y(o.min_y), max_x(o.max_x), max_y(o.max_y) {}
};

extern const f32 g_unk_710164be30;
f32 fn_7100217ee0(f32 a, f32 b);

static inline f32 DegToRad(f32 deg) {
    return (f32)((double)(deg * 0.5f) * 3.14159265358979323846 / 180.0);
}

// NON_MATCHING(minor): the left-edge product is in s3 instead of s0 (register choice only)
UnkMinimapRect fn_7100e94e10() {
    UnkMinimapRect r;
    r.min_x = 3.4028235e+38f;
    r.min_y = 3.4028235e+38f;
    r.max_x = -3.4028235e+38f;
    r.max_y = -3.4028235e+38f;

    UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310);
    if (!target) return r;
    UnkViewCamera* cam = ((UnkViewOwner*)target->field_0x248)->field_0x40;
    if (!cam) return r;

    f32 z;
    {
        UnkVec3f pos = fn_7100206154(&cam->xform);
        z = pos.z;
    }
    f32 half_v = DegToRad(cam->field_0x130->field_0x80);
    f32 pitch = asinf(cam->field_0x130->field_0xb4);
    const f32 lim = 0.7853982f;
    pitch = !(pitch <= -lim) ? (!(pitch >= lim) ? pitch : lim) : -lim;
    f32 near_z = z * tanf(pitch - half_v);
    f32 far_z = z * tanf(pitch + half_v);
    f32 margin = g_unk_710164be30;
    f32 bottom = floorf(near_z + margin);
    f32 top = floorf(far_z - margin);
    f32 h = DegToRad(fn_7100217ee0(cam->field_0x130->field_0x80, 1.7777778f));
    f32 yaw = asinf(cam->field_0x130->field_0xb8);
    f32 spread = z * (fabsf(tanf(pitch * 0.5f)) + 1.0f);
    f32 right_z = tanf(yaw + h) * spread;
    f32 left_z = tanf(yaw - h) * spread;

    f32 right = floorf(right_z - margin);
    f32 left = floorf(margin + left_z);

    f32 x0, y0, x1, y1;
    {
        UnkVec3f px = fn_7100206154(&cam->xform);
        x0 = bottom + px.x;
        UnkVec3f py = fn_7100206154(&cam->xform);
        y0 = left + py.y;
    }
    r.min_x = x0;
    {
        UnkVec3f px2 = fn_7100206154(&cam->xform);
        x1 = top + px2.x;
        UnkVec3f py2 = fn_7100206154(&cam->xform);
        y1 = right + py2.y;
    }
    r.min_y = y0 - margin * 0.3f;
    r.max_x = x1 + 0.0f;
    r.max_y = margin * 0.3f + y1;
    return r;
}
