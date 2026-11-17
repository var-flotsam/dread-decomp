// Unit 259: projects the near and far faces of a box, seen along a direction from a point, and
// returns their depths mapped to [0, 1].
// Names and layouts are placeholders.
#include "types.h"
#include "base/math/CVector4D.h"

struct UnkVec4f {
    f32 x, y, z, w;
    UnkVec4f() {}
    UnkVec4f(const UnkVec4f& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct UnkMat4f {
    f32 m[4][4];
};

UnkVec4f fn_7100078218(const UnkMat4f* m, const UnkVec4f* v);

struct UnkVec3_1c75e0 {
    f32 x, y, z;
};

struct UnkDepthBox_1c75e0 {
    u8 pad_0x0[0x24];
    UnkVec3_1c75e0 center;  // 0x24
    UnkVec3_1c75e0 extent;  // 0x30
    u8 pad_0x3c[0x7c - 0x3c];
    f32 field_0x7c;
    f32 field_0x80;
};

void fn_71001c75e0(const UnkDepthBox_1c75e0* box, const UnkVec3_1c75e0* point,
                   const UnkVec3_1c75e0* dir, const UnkMat4f* viewProj, f32* outNear, f32* outFar) {
    f32 minX = box->center.x - box->extent.x;
    f32 minY = box->center.y - box->extent.y;
    f32 minZ = box->center.z - box->extent.z;
    f32 maxX = box->center.x + box->extent.x;
    f32 maxY = box->center.y + box->extent.y;
    f32 maxZ = box->center.z + box->extent.z;

    f32 dMin = (minX - point->x) * dir->x + (minY - point->y) * dir->y + (minZ - point->z) * dir->z;
    f32 dMax = (maxX - point->x) * dir->x + (maxY - point->y) * dir->y + (maxZ - point->z) * dir->z;

    if (!(dMin > 0.0f) && !(dMax > 0.0f)) {
        *outFar = 0.0f;
        *outNear = 0.0f;
        return;
    }

    f32 farX, farY, farZ;
    f32 nearX, nearY, nearZ;
    bool behind;
    if (!(dMin > 0.0f)) {
        farX = maxX;
        farY = maxY;
        farZ = maxZ;
        nearX = 0.0f;
        nearY = 0.0f;
        nearZ = 0.0f;
        behind = true;
    } else if (dMax < 0.0f) {
        farX = minX;
        farY = minY;
        farZ = minZ;
        nearX = 0.0f;
        nearY = 0.0f;
        nearZ = 0.0f;
        behind = true;
    } else {
        // Both faces are in front: the nearer one is the near face.
        behind = false;
        if (dMin < dMax) {
            farX = maxX;
            farY = maxY;
            farZ = maxZ;
            nearX = minX;
            nearY = minY;
            nearZ = minZ;
        } else {
            farX = minX;
            farY = minY;
            farZ = minZ;
            nearX = maxX;
            nearY = maxY;
            nearZ = maxZ;
        }
    }
    f32 offset = box->field_0x7c < box->field_0x80 ? box->field_0x80 : box->field_0x7c + 0.001f;
    f32 dx = dir->x * offset;
    f32 dy = dir->y * offset;
    f32 dz = dir->z * offset;
    nearX -= dx;
    nearY -= dy;
    nearZ -= dz;
    farX += dx;
    farY += dy;
    farZ += dz;

    UnkVec4f nearClip;
    {
        UnkVec4f nearPoint;
        nearPoint.x = nearX;
        nearPoint.y = nearY;
        nearPoint.z = nearZ;
        nearPoint.w = 1.0f;
        nearClip = fn_7100078218(viewProj, &nearPoint);
    }
    UnkVec4f farPoint;
    farPoint.x = farX;
    farPoint.y = farY;
    farPoint.z = farZ;
    farPoint.w = 1.0f;
    UnkVec4f farClip = fn_7100078218(viewProj, &farPoint);

    behind |= nearClip.w < 0.0f;
    fn_710007b568(reinterpret_cast<base::math::CVector4D*>(&nearClip), nearClip.w);
    fn_710007b568(reinterpret_cast<base::math::CVector4D*>(&farClip), farClip.w);

    *outNear = nearClip.z;
    *outFar = farClip.z * 0.5f + 0.5f;
    f32 nearDepth = *outNear * 0.5f + 0.5f;
    *outNear = behind ? 0.0f : (nearDepth > 0.0f ? nearDepth : 0.0f);
    f32 farDepth = *outFar > *outNear ? *outFar : *outNear;
    *outFar = farDepth < 1.0f ? farDepth : 1.0f;
}