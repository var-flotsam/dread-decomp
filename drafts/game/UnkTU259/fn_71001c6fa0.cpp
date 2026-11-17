// Unit 259: the depth range a light's cone covers, seen along a direction from a point: the cone's
// apex, its far rim and the point behind it are projected and the nearest and farthest kept,
// mapped to [0, 1]. Names and layouts are placeholders.
#include <math.h>
#include "base/math/CVector4D.h"
#include "types.h"

struct UnkVec4f {
    f32 x, y, z, w;
    UnkVec4f() {}
    UnkVec4f(const UnkVec4f& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkMat4f {
    f32 m[4][4];
};

UnkVec4f fn_7100078218(const UnkMat4f* m, const UnkVec4f* v);

struct UnkConeVec3_l1c6fa0 {
    f32 x, y, z;
};

static inline UnkConeVec3_l1c6fa0 Add(const UnkConeVec3_l1c6fa0& a, const UnkConeVec3_l1c6fa0& b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
static inline UnkConeVec3_l1c6fa0 Sub(const UnkConeVec3_l1c6fa0& a, const UnkConeVec3_l1c6fa0& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
static inline UnkConeVec3_l1c6fa0 Scale(const UnkConeVec3_l1c6fa0& a, f32 s) {
    return {a.x * s, a.y * s, a.z * s};
}
static inline f32 Dot(const UnkConeVec3_l1c6fa0& a, const UnkConeVec3_l1c6fa0& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static inline UnkConeVec3_l1c6fa0 Cross(const UnkConeVec3_l1c6fa0& a, const UnkConeVec3_l1c6fa0& b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
static inline f32 Length(const UnkConeVec3_l1c6fa0& a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

struct UnkConeLight_l1c6fa0 {
    u8 pad_0x0[0x24];
    UnkConeVec3_l1c6fa0 position;   // 0x24
    UnkConeVec3_l1c6fa0 direction;  // 0x30
    u8 pad_0x3c[0x7c - 0x3c];
    f32 range_near;   // 0x7c
    f32 range_far;    // 0x80
    f32 angle_inner;  // 0x84
    f32 angle_outer;  // 0x88
    f32 field_0x8c;
    f32 field_0x90;

    // The far end of the range, kept above the near end.
    f32 Range() const { return range_near < range_far ? range_far : range_near + 0.001f; }
    f32 Angle() const { return angle_inner < angle_outer ? angle_outer : angle_inner + 0.001f; }
    f32 Limit() const { return field_0x8c > field_0x90 ? field_0x90 : field_0x8c; }
};

void fn_71001c6fa0(const UnkConeLight_l1c6fa0* light, const UnkConeVec3_l1c6fa0* point,
                   const UnkConeVec3_l1c6fa0* view, const UnkMat4f* viewProj, f32* outNear, f32* outFar) {
    f32 facing = Dot(light->direction, *view);
    f32 range = light->Range();
    UnkConeVec3_l1c6fa0 apex =
        Add(light->position, Scale(Scale(light->direction, range), light->Limit()));
    UnkConeVec3_l1c6fa0 back;
    if (facing <= 0.0f)
        back = Sub(light->position, Scale(*view, range));
    else
        back = Add(light->position, Scale(*view, range));
    f32 radius = sinf((f64)light->Angle() * 3.141592653589793 / 180.0) * light->Range();
    UnkConeVec3_l1c6fa0 side = Scale(Cross(Cross(*view, light->direction), light->direction), radius);
    UnkConeVec3_l1c6fa0 rim[2] = {Add(apex, side), Sub(apex, side)};

    f32 rimDepth0 = Dot(Sub(rim[0], *point), *view);
    f32 rimDepth1 = Dot(Sub(rim[1], *point), *view);
    f32 originDepth = Dot(Sub(light->position, *point), *view);
    f32 backDepth = Dot(Sub(back, *point), *view);
    f32 rimDistance0 = Length(Scale(*view, rimDepth0));
    f32 rimDistance1 = Length(Scale(*view, rimDepth1));
    f32 originDistance = Length(Scale(*view, originDepth));
    f32 backDistance = Length(Scale(*view, backDepth));

    UnkConeVec3_l1c6fa0 points[4] = {};
    f32 distances[4];
    s32 count = 0;
    if (backDepth >= 0.0f && Dot(Sub(back, *point), light->direction) <= light->Limit()) {
        points[count] = back;
        distances[count] = backDistance;
        count++;
    }
    if (rimDepth0 >= 0.0f) {
        points[count] = rim[0];
        distances[count] = rimDistance0;
        count++;
    }
    if (rimDepth1 >= 0.0f) {
        points[count] = rim[1];
        distances[count] = rimDistance1;
        count++;
    }
    if (originDepth >= 0.0f) {
        points[count] = light->position;
        distances[count] = originDistance;
        count++;
    }
    if (count == 0) {
        *outNear = 0.0f;
        *outFar = 0.0f;
        return;
    }

    f32 nearest = 3.4028235e38f;
    f32 farthest = 0.0f;
    s32 nearIndex = -1;
    s32 farIndex = -1;
    for (s32 i = 0; i < count; i++) {
        f32 d = distances[i];
        if (d < nearest) {
            nearest = d;
            nearIndex = i;
        }
        if (d > farthest) {
            farthest = d;
            farIndex = i;
        }
    }

    UnkConeVec3_l1c6fa0 margin = Scale(*view, 100.0f);
    UnkConeVec3_l1c6fa0 nearPoint = Sub(points[nearIndex], margin);
    UnkConeVec3_l1c6fa0 farPoint = Add(points[farIndex], margin);

    UnkVec4f nearClip;
    {
        UnkVec4f in;
        in.x = nearPoint.x;
        in.y = nearPoint.y;
        in.z = nearPoint.z;
        in.w = 1.0f;
        nearClip = fn_7100078218(viewProj, &in);
    }
    UnkVec4f in;
    in.x = farPoint.x;
    in.y = farPoint.y;
    in.z = farPoint.z;
    in.w = 1.0f;
    UnkVec4f farClip = fn_7100078218(viewProj, &in);

    f32 nearZ = nearClip.z;
    fn_710007b568(reinterpret_cast<base::math::CVector4D*>(&nearClip), nearClip.w);
    fn_710007b568(reinterpret_cast<base::math::CVector4D*>(&farClip), farClip.w);

    *outNear = nearClip.z;
    *outFar = farClip.z * 0.5f + 0.5f;
    f32 nearDepth = count < 4 ? 0.0f : *outNear * 0.5f + 0.5f;
    nearDepth = nearDepth > 0.0f ? nearDepth : 0.0f;
    *outNear = nearZ < 0.0f ? 0.0f : nearDepth;
    f32 farDepth = *outFar > *outNear ? *outFar : *outNear;
    *outFar = farDepth < 1.0f ? farDepth : 1.0f;
}
