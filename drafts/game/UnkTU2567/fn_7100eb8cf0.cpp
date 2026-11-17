// Unit 2567 (the minimap's meshes): adds a rectangle to three draw lists, its four corners to the
// vertices, its indices (two triangles when filled, else the four edges as lines) and its colour
// once per corner. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "game/UnkDashMesh.h"
#include "types.h"

using base::global::CRntVector;

// As src/game/CMinimapManager.cpp declares them.
struct UnkMinimapVec3 {
    f32 x;
    f32 y;
    f32 z;
    UnkMinimapVec3() {}
    UnkMinimapVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkMinimapVec3& operator=(const UnkMinimapVec3& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        return *this;
    }
    UnkMinimapVec3 operator*(f32 s) const { return UnkMinimapVec3(x * s, y * s, z * s); }
    UnkMinimapVec3(const UnkMinimapVec3& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
    }
};

struct UnkMinimapColor : base::math::CVector4D {
    UnkMinimapColor() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }
    UnkMinimapColor(const UnkMinimapVec3& rgb, f32 a_) {
        x = rgb.x;
        y = rgb.y;
        z = rgb.z;
        w = a_;
    }
    UnkMinimapColor(const UnkMinimapColor& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
        ((a_u32*)this)[2] = ((const a_u32*)&o)[2];
        ((a_u32*)this)[3] = ((const a_u32*)&o)[3];
    }
};

struct UnkMinimapDrawList {
    void* data;
    u32 count;
    u8 unk_0xc[0x1c];
};

// Appends a vertex (the vertex array's out-of-line PushBack).
void fn_7100149810(base::global::CRntVector<UnkMeshVertex>* self, const UnkMeshVertex* v);

// The centre plus an offset.
static inline UnkMeshVertex Offset(const UnkMeshVertex& center, f32 dx, f32 dy, f32 dz) {
    return UnkMeshVertex(center.x + dx, center.y + dy, center.z + dz);
}

// `rect` is min x, min y, max x, max y; the corners are at depth `depth`.
void fn_7100eb8cf0(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   const void* rect, UnkMinimapColor color, bool filled, f32 depth) {
    CRntVector<UnkMeshVertex>* vertices = reinterpret_cast<CRntVector<UnkMeshVertex>*>(lists0);
    CRntVector<u16>* indices = reinterpret_cast<CRntVector<u16>*>(lists3);
    CRntVector<UnkMinimapColor>* colors = reinterpret_cast<CRntVector<UnkMinimapColor>*>(lists1);
    const f32* r = static_cast<const f32*>(rect);
    f32 min_x = r[0];
    f32 min_y = r[1];
    f32 max_x = r[2];
    f32 max_y = r[3];

    u32 base = vertices->count;
    indices->PushBack(base);
    if (filled) {
        // Two triangles: 0 3 1 and 1 3 2.
        u16 i3 = base + 3;
        u16 i1 = base + 1;
        indices->PushBack(i3);
        indices->PushBack(i1);
        indices->PushBack(i1);
        indices->PushBack(i3);
        indices->PushBack(base + 2);
    } else {
        // The four edges as lines.
        indices->PushBack(base + 1);
        indices->PushBack(base + 1);
        indices->PushBack(base + 2);
        indices->PushBack(base + 2);
        indices->PushBack(base + 3);
        indices->PushBack(base + 3);
        indices->PushBack(base);
    }

    UnkMeshVertex center((min_x + max_x) * 0.5f, (min_y + max_y) * 0.5f, depth);
    f32 half_w = (max_x - min_x) * 0.5f;
    f32 half_h = (max_y - min_y) * 0.5f;
    UnkMeshVertex v = Offset(center, -half_w, half_h, 0.0f);
    fn_7100149810(vertices, &v);
    v = Offset(center, half_w, half_h, 0.0f);
    fn_7100149810(vertices, &v);
    v = Offset(center, half_w, -half_h, 0.0f);
    fn_7100149810(vertices, &v);
    v = Offset(center, -half_w, -half_h, 0.0f);
    fn_7100149810(vertices, &v);

    colors->PushBackCopy(color);
    colors->PushBackCopy(color);
    colors->PushBackCopy(color);
    colors->PushBackCopy(color);
}
