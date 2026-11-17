// In unit 2567 of tools/tu_map.py (the minimap's items): appends an icon's quad to the four draw lists: two
// triangles' indices, four corners around `pos` (scaled by `size`, offset by the icon's pivot, rotated by
// `angle`), the colour and the icon's cell of the atlas. Names and layouts are placeholders.
#include <math.h>
#include <utility>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkDashMesh.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// The vectors and the colour copy word by word; their user-provided copy constructors make
// by-value arguments go through a temporary passed by address.
struct UnkMinimapVec2 {
    f32 x;
    f32 y;
    UnkMinimapVec2() {}
    UnkMinimapVec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkMinimapVec2(const UnkMinimapVec2& o) {
        ((a_u32*)this)[0] = ((const a_u32*)&o)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&o)[1];
    }
};

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

// The colour is a vector (its assignment is the vector's, fn_710007b518).
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

// An item's icon definition: whether it is sized from the item's box (0xb0), which of the
// owner's two scales it uses (0xb1), and four margins that must all be positive for the boxed
// drawing.
struct UnkMinimapIconDef {
    u8 unk_0x0[0xb0];
    bool sized_by_box;
    bool alt_scale;
    u8 unk_0xb2[0x2];
    f32 margins[4];
};

// A draw list (0x28 bytes): its entries and their count, reset at the start of each pass.
struct UnkMinimapDrawList {
    void* data;
    u32 count;
    u8 unk_0xc[0x1c];
};

// What this function reads of an icon definition: its kind, its cell in the atlas (8 rows of 16
// columns) and its pivot.
struct UnkMinimapIconView_leb9b90 {
    CStrId kind;
    u32 row;     // 0x8
    u32 column;  // 0xc
    u8 pad_0x10[0x98];
    f32 pivot_x;  // 0xa8
    f32 pivot_y;  // 0xac
};
static_assert(__builtin_offsetof(UnkMinimapIconView_leb9b90, pivot_x) == 0xa8, "layout");

extern CStrId g_unk_71727c8070;
extern CStrId g_unk_71727c8390;

// Appends a vertex (the vertex array's out-of-line PushBack).
void fn_7100149810(base::global::CRntVector<UnkMeshVertex>* self, const UnkMeshVertex* v);

// Appends a copy of `value`, growing as CRntVector::PushBackCopy does (including its hang).
template <class T>
inline void Append(CRntVector<T>* array, const T& value) {
    u32 n = array->count;
    u32 capacity = array->capacity;
    u32 grown = capacity;
    if (capacity <= n && array->step != 0)
        grown = (n / array->step + 1) * array->step;
    if ((s32)n >= (s32)grown)
        for (;;) {
        }
    array->Reserve(grown);
    new (&array->data[array->count]) T(value);
    ++array->count;
}

void fn_7100eb9b90(UnkMinimapDrawList* lists0, UnkMinimapDrawList* lists3, UnkMinimapDrawList* lists1,
                   UnkMinimapDrawList* lists2, const UnkMinimapIconDef* icon, UnkMinimapVec3 pos,
                   f32 size, UnkMinimapColor color, f32 angle, bool flip_u, bool flipped) {
    auto* positions = reinterpret_cast<CRntVector<UnkMeshVertex>*>(lists0);
    auto* indices = reinterpret_cast<CRntVector<u16>*>(lists3);
    auto* colors = reinterpret_cast<CRntVector<UnkMinimapColor>*>(lists1);
    auto* uvs = reinterpret_cast<CRntVector<UnkMinimapVec2>*>(lists2);
    const auto* view = reinterpret_cast<const UnkMinimapIconView_leb9b90*>(icon);

    u16 first = positions->count;
    indices->PushBack(first);
    indices->PushBack(first + 3);
    indices->PushBack(first + 1);
    indices->PushBack(first + 1);
    indices->PushBack(first + 3);
    indices->PushBack(first + 2);

    f32 center_x = view->pivot_x * size;
    f32 center_y = (flipped ? -view->pivot_y : view->pivot_y) * size;
    f32 left = center_x - size;
    f32 right = center_x + size;
    f32 top = center_y + size;
    f32 bottom = center_y - size;

    f32 x0 = left, y0 = top;
    f32 x1 = right, y1 = top;
    f32 x2 = right, y2 = bottom;
    f32 x3 = left, y3 = bottom;
    if (angle != 0.0f) {
        f32 s = sinf(angle);
        f32 c = cosf(angle);
        x0 = left * c - top * s;
        y0 = left * s + top * c;
        x1 = right * c - top * s;
        y1 = right * s + top * c;
        x2 = right * c - bottom * s;
        y2 = right * s + bottom * c;
        x3 = left * c - bottom * s;
        y3 = left * s + bottom * c;
    }

    auto add_corner = [&](f32 x, f32 y) {
        UnkMeshVertex corner(x + pos.x, y + pos.y, 0.0f + pos.z);
        fn_7100149810(positions, &corner);
    };
    add_corner(x0, y0);
    add_corner(x1, y1);
    add_corner(x2, y2);
    add_corner(x3, y3);

    Append(colors, color);
    Append(colors, color);
    Append(colors, color);
    Append(colors, color);

    f32 v0 = (f32)view->row * 0.125f;
    f32 u0 = (f32)view->column * 0.0625f;
    f32 u1 = u0 + 0.0625f;
    f32 v1 = v0 + 0.125f;
    // Inset the cell vertically, except for two kinds of icon.
    if (view->kind.data != g_unk_71727c8390.data && view->kind.data != g_unk_71727c8070.data) {
        v0 += 0.0025f;
        v1 -= 0.0025f;
    }
    UnkMinimapVec2 top_left(u0, v0);
    UnkMinimapVec2 top_right(u1, v0);
    UnkMinimapVec2 bottom_right(u1, v1);
    UnkMinimapVec2 bottom_left(u0, v1);
    if (flip_u) {
        std::swap(top_left, top_right);
        std::swap(bottom_left, bottom_right);
    }
    if (flipped) {
        std::swap(top_left, bottom_left);
        std::swap(top_right, bottom_right);
    }
    Append(uvs, top_left);
    Append(uvs, top_right);
    Append(uvs, bottom_right);
    Append(uvs, bottom_left);
}
