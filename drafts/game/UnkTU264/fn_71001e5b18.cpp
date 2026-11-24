// Unit 264 (tools/tu_map.py, no name hint): fits a directional light's shadow volume around a focus point.
// Builds a view basis from the light's direction, gathers the casters, bounds a fixed box around the focus in
// that basis and returns the light's position, depth range and extents. Names and layouts are placeholders.
#include <float.h>
#include <math.h>

#include "base/global/CRntVector.h"
#include "game/UnkTU251.h"
#include "game/UnkTU54.h"
#include "types.h"

struct UnkMat4f {
    f32 m[4][4];
};

struct UnkShadowLight_l1e5b18 {
    u8 pad_0x0[0x30];
    f32 direction[3];  // 0x30
    u8 pad_0x3c[0x40];
    f32 field_0x7c;
    f32 field_0x80;
    u8 pad_0x84[0x134];
    bool dirty;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkShadowLight_l1e5b18, dirty) == 0x1b8, "UnkShadowLight_l1e5b18 layout");

struct UnkShadowHolder_l1e5b18 {
    u8 pad_0x0[0x78];
    UnkShadowLight_l1e5b18* light;  // 0x78
};

struct UnkCasterInfo_l1e5b18 {
    u8 pad_0x0[0x35c];
    bool castsShadow;  // 0x35c
};

struct UnkCaster_l1e5b18 {
    UnkCasterInfo_l1e5b18* info;
};

struct UnkCasterList_l1e5b18 {
    UnkCaster_l1e5b18** data;
    u32 count;
    u32 pad_0xc;
};

struct UnkShadowFit_l1e5b18 {
    u8 pad_0x0[0x10];
    UnkShadowHolder_l1e5b18* holder;  // 0x10
    u8 pad_0x18[0x20];
    UnkCasterList_l1e5b18 allCasters;     // 0x38
    UnkCasterList_l1e5b18 shadowCasters;  // 0x48
    u8 pad_0x58[0x60];
    f32 field_0xb8;
    f32 field_0xbc;
    u8 pad_0xc0[0x18];
    base::global::CRntVector<UnkCaster_l1e5b18*> casters;        // 0xd8
    base::global::CRntVector<UnkCaster_l1e5b18*> castersShadow;  // 0x100
};
static_assert(__builtin_offsetof(UnkShadowFit_l1e5b18, casters) == 0xd8, "UnkShadowFit_l1e5b18 layout");
static_assert(__builtin_offsetof(UnkShadowFit_l1e5b18, castersShadow) == 0x100, "UnkShadowFit_l1e5b18 layout");

struct UnkShadowFocus_l1e5b18 {
    u8 pad_0x0[0xa8];
    f32 x;  // 0xa8
    f32 y;  // 0xac
};

static inline bool NearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

// a x b into (x, y, z).
static inline void Cross(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz, f32& x, f32& y, f32& z) {
    x = ay * bz - az * by;
    y = az * bx - ax * bz;
    z = ax * by - ay * bx;
}

static inline UnkVec3f operator*(const UnkMat4f& m, const UnkVec3f& v) {
    return TransformPointByMatrixWithPerspectiveDivide(&m, &v);
}

void fn_71001e5b18(UnkShadowFit_l1e5b18* self, UnkShadowFocus_l1e5b18* focus, UnkVec3f* outPos, f32* outNear,
                   f32* outFar, f32* outWidth, f32* outHeight) {
    UnkShadowLight_l1e5b18* light = self->holder->light;

    // The light's direction, normalised.
    f32 len = sqrtf(light->direction[0] * light->direction[0] + light->direction[1] * light->direction[1] +
                    light->direction[2] * light->direction[2]);
    f32 inv = 1.0f / (len + 0.0001f);
    f32 dx = light->direction[0] * inv;
    f32 dy = light->direction[1] * inv;
    f32 dz = light->direction[2] * inv;

    // Right: an axis that is not parallel to the direction, crossed with it.
    f32 sx, sy, sz;
    if (NearZero(dy))
        Cross(0.0f, 1.0f, 0.0f, dx, dy, dz, sx, sy, sz);
    else if (NearZero(dx))
        Cross(1.0f, 0.0f, 0.0f, dx, dy, dz, sx, sy, sz);
    else
        Cross(0.0f, 0.0f, 1.0f, dx, dy, dz, sx, sy, sz);
    f32 sinv = 1.0f / (sqrtf(sx * sx + sy * sy + sz * sz) + 0.0001f);
    f32 rx = sx * sinv;
    f32 ry = sy * sinv;
    f32 rz = sz * sinv;

    // Up: direction x right.
    f32 tx = dy * rz - dz * ry;
    f32 ty = dz * rx - dx * rz;
    f32 tz = dx * ry - dy * rx;
    f32 tinv = 1.0f / (sqrtf(tx * tx + ty * ty + tz * tz) + 0.0001f);
    f32 ux = tx * tinv;
    f32 uy = ty * tinv;
    f32 uz = tz * tinv;

    UnkMat4f view = {};
    view.m[0][0] = rx;
    view.m[0][1] = ry;
    view.m[0][2] = rz;
    view.m[1][0] = ux;
    view.m[1][1] = uy;
    view.m[1][2] = uz;
    view.m[2][0] = dx;
    view.m[2][1] = dy;
    view.m[2][2] = dz;
    view.m[3][3] = 1.0f;

    f32 focusX = focus->x;
    f32 focusY = focus->y;

    u32 count = self->allCasters.count;
    for (u32 i = 0; i < count; ++i) {
        UnkCaster_l1e5b18* caster = self->allCasters.data[i];
        self->casters.PushBack(caster);
    }

    u32 shadowCount = self->shadowCasters.count;
    for (u32 i = 0; i < shadowCount; ++i) {
        UnkCaster_l1e5b18* caster = self->shadowCasters.data[i];
        if (caster && caster->info->castsShadow)
            self->castersShadow.PushBack(caster);
    }

    // The box around the focus, in the light's view.
    f32 right = focusX + 2000.0f;
    f32 left = focusX + -2000.0f;
    f32 top = focusY + 500.0f;
    f32 bottom = focusY + -500.0f;
    UnkVec3f corners[8];
    corners[0] = view * UnkVec3f(right, top, 500.0f);
    corners[1] = view * UnkVec3f(right, top, -1000.0f);
    corners[2] = view * UnkVec3f(right, bottom, 500.0f);
    corners[3] = view * UnkVec3f(right, bottom, -1000.0f);
    corners[4] = view * UnkVec3f(left, top, 500.0f);
    corners[5] = view * UnkVec3f(left, top, -1000.0f);
    corners[6] = view * UnkVec3f(left, bottom, 500.0f);
    corners[7] = view * UnkVec3f(left, bottom, -1000.0f);

    f32 maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
    f32 minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    for (int i = 0; i < 8; ++i) {
        maxX = corners[i].x > maxX ? corners[i].x : maxX;
        maxY = corners[i].y > maxY ? corners[i].y : maxY;
        maxZ = corners[i].z > maxZ ? corners[i].z : maxZ;
        minX = corners[i].x < minX ? corners[i].x : minX;
        minY = corners[i].y < minY ? corners[i].y : minY;
        minZ = corners[i].z < minZ ? corners[i].z : minZ;
    }

    UnkShadowHolder_l1e5b18* holder = self->holder;
    holder->light->field_0x80 = 3800.0f;
    holder->light->dirty = true;
    fn_71001bdc3c(holder);

    f32 sumX = minX + maxX;
    f32 sumY = minY + maxY;
    f32 sumZ = minZ + maxZ;
    f32 depth = maxZ - minZ;

    self->holder->light->field_0x7c = 3500.0f;
    *outWidth = maxX - minX;
    *outHeight = maxY - minY;
    self->field_0xb8 = 200.0f;
    self->field_0xbc = depth + 200.0f;
    *outNear = 200.0f;
    *outFar = self->field_0xbc > 50000.0f ? self->field_0xbc : 50000.0f;

    // The box's centre back in world space, pulled back along the direction past its near face.
    f32 cx = rx * 0.5f * sumX + ux * 0.5f * sumY + dx * 0.5f * sumZ;
    f32 cy = ry * 0.5f * sumX + uy * 0.5f * sumY + dy * 0.5f * sumZ;
    f32 cz = rz * 0.5f * sumX + uz * 0.5f * sumY + dz * 0.5f * sumZ;
    f32 back = depth * 0.5f + 400.0f;
    outPos->x = cx - dx * back;
    outPos->y = cy - dy * back;
    outPos->z = cz - dz * back;
}
