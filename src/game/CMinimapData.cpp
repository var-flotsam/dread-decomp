// From unit 2563 of tools/tu_map.py (0x7100e31c18-0x7100e65204, CMinimapData by reflection): the
// minimap data's arrays. Names and layouts are placeholders.
// Holds fn_7100e62e24 (resizes an array of 0x138-byte entries) and fn_7100e3da0c (places a map
// marker's segment).
#include <math.h>
#include <string.h>

#include <new>

#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The entry's out-of-line special members (declared as the generated src/clones/ ones are): the
// move constructor, the destructor and the implicit default constructor run after zeroing.
void fn_7100e51078(void* dst, void* src);
void fn_7100e4e310(char* self);
void fn_7100e50c98(void* self);

// An entry of the array (0x138 bytes; its members are not spelled out).
struct UnkMinimapEntry_0x138 {
    u8 unk_0x0[0x138];

    UnkMinimapEntry_0x138(UnkMinimapEntry_0x138&& other) { fn_7100e51078(this, &other); }
    ~UnkMinimapEntry_0x138() { fn_7100e4e310(reinterpret_cast<char*>(this)); }
};
static_assert(sizeof(UnkMinimapEntry_0x138) == 0x138, "UnkMinimapEntry_0x138 layout");

// Resizes the array to `n` entries: the array grows to exactly `n` when it is too small (the
// entries are moved over), and the new entries are value-initialised (zeroed, then constructed).
// Shrinking only lowers the count.
void fn_7100e62e24(base::UnkVector<UnkMinimapEntry_0x138>* self, u32 n) {
    self->Reserve(n);
    for (u32 i = self->count; i < n; ++i) {
        UnkMinimapEntry_0x138* entry = &self->data[i];
        memset(entry, 0, sizeof(*entry));
        fn_7100e50c98(entry);
    }
    self->count = n;
}

// A point of the marker's path, handed back in memory (a user-provided destructor makes it so).
struct UnkMinimapPathPoint {
    CVector3D v;
    ~UnkMinimapPathPoint() {}
};
UnkMinimapPathPoint fn_71010596ac(void* path, u32 index, f32 t);
static inline void Project(CVector2D* out, const UnkMinimapPathPoint& point) {
    fn_710007b430(out, &point.v);
}
void fn_7100ebda30(void* path, void* arg1, u32 arg2, f32 a, f32 b, f32 c, f32 d);
void fn_7100ebc7f0(const UnkSegment2D* seg, void* out, f32 a, f32 b);
UnkSegment2D* fn_7100125b44(UnkSegment2D* self, const UnkSegment2D* other);

// A map marker (placeholder names): its kind, its path at +0x10, the flag that reverses its
// segment, the two points at +0x64 and what fn_7100ebc7f0 fills at +0xd8.
struct UnkMinimapMarker {
    u8 unk_0x0[0x8];
    s32 kind;
    u8 unk_0xc[0x4];
    u8 path[0x50];
    bool reversed;  // +0x60
    u8 unk_0x61[0x3];
    CVector2D a;  // +0x64
    CVector2D b;  // +0x6c
    u8 unk_0x74[0x4];
    u8 field_0x78[0x60];
    u8 field_0xd8[1];
};
static_assert(__builtin_offsetof(UnkMinimapMarker, field_0xd8) == 0xd8, "UnkMinimapMarker placeholder layout");

// Builds the marker's segment: for kind 2 the segment between its two points moved 25 units along
// their normal; otherwise one from the first point of its path, along the path's first step, as
// long as the two points are apart. Reversed when the marker says so, it goes to fn_7100ebc7f0.
// The edge is computed in each branch and the normal before the length, as the original's blocks
// are laid out.
// NON_MATCHING(minor): register allocation and scheduling only: for kind 2 the two squares are
// multiplied in the other order, the offset and the new points land in other s registers and the
// first -1.0 store is scheduled before the multiplies; in the other branch the end point's
// coordinates are computed in swapped registers.
void fn_7100e3da0c(UnkMinimapMarker* self) {
    fn_7100ebda30(self->path, self->field_0x78, 0, 35.0f, 80.0f, 27.0f, 400.0f);
    CVector2D p0, p1;
    Project(&p0, fn_71010596ac(self->path, 0, 0.0f));
    Project(&p1, fn_71010596ac(self->path, 1, 0.0f));
    f32 dx = p1.x - p0.x;
    f32 dy = p1.y - p0.y;
    f32 length = sqrtf(dx * dx + dy * dy);
    if (length != 0.0f) {
        dx /= length;
        dy /= length;
    }
    UnkSegment2D seg;
    seg.start = {0.0f, 0.0f};
    seg.end = {0.0f, 0.0f};
    seg.field_0x10 = -1.0f;
    if (self->kind == 2) {
        f32 ex = self->b.x - self->a.x;
        f32 ey = self->b.y - self->a.y;
        f32 nx = -ey;
        f32 ny = ex;
        f32 d = sqrtf(ex * ex + ey * ey);
        if (d != 0.0f) {
            nx = -ey / d;
            ny = ex / d;
        }
        nx *= 25.0f;
        ny *= 25.0f;
        seg.field_0x10 = -1.0f;
        seg.start = {self->a.x - nx, self->a.y - ny};
        seg.end = {self->b.x - nx, self->b.y - ny};
        seg.field_0x10 = -1.0f;
    } else {
        f32 ex = self->b.x - self->a.x;
        f32 ey = self->b.y - self->a.y;
        f32 d = sqrtf(ex * ex + ey * ey);
        seg.field_0x10 = -1.0f;
        seg.start.x = p0.x;
        seg.start.y = p0.y;
        seg.end = {dx * d + p0.x, dy * d + p0.y};
        seg.field_0x10 = -1.0f;
    }
    if (self->reversed) {
        UnkSegment2D r(seg.end, seg.start);
        fn_7100125b44(&seg, &r);
    }
    fn_7100ebc7f0(&seg, self->field_0xd8, 50.0f, 500.0f);
}
