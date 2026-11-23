#pragma once

#include "base/math/UnkSegment2D.h"
#include "types.h"

// What the 2D tests fill on a hit (the layout of src/game/malloc/fn_710012be70.cpp's contact): the
// point, the normal of the first shape, the normal of the second, and which of them are set.
struct UnkContact2D {
    base::math::CVector3D point;
    base::math::CVector3D normal_a;
    base::math::CVector3D normal_b;
    f32 param;
    u32 flags;
    void SetPoint(f32 x, f32 y) {
        point.x = x;
        point.y = y;
        point.z = 0.0f;
        flags |= 8;
    }
    void SetParam(f32 t) {
        param = t;
        flags |= 0x200;
    }
    void SetNormalA(const base::math::CVector2D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const base::math::CVector2D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
    void SetPoint(const base::math::CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const base::math::CVector3D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = n.z;
        flags |= 0x20;
    }
    void SetNormalB(const base::math::CVector3D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = n.z;
        flags |= 0x10;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};
