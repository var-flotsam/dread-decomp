// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py unit 2515): reads the shape's
// outlines from a file (a count, then per outline its point count, z, points, closed flag and bounds),
// then the shape's bounds, and lets the shape update itself. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU108.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;

// As src/game/CBreakableTileGroup.cpp defines them.
struct UnkTilePoint {
    f32 x, y, z;
};
struct UnkTileOutline {
    base::global::CRntVector<UnkTilePoint> points;
    f32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    base::math::CVector2D min;
    base::math::CVector2D max;
    u32 field_0x40;

    UnkTileOutline()
        : points(7), field_0x28(0.0f), field_0x2c(false), field_0x2d(true), min{3.4028235e38f, 3.4028235e38f},
          max{-3.4028235e38f, -3.4028235e38f}, field_0x40(0) {}
    ~UnkTileOutline() { points.Free(); }
};
static_assert(sizeof(UnkTileOutline) == 0x48, "UnkTileOutline placeholder layout");

// Grows the outline array's storage to at least `capacity` (out of line).
void fn_710011f1ec(CRntVector<UnkTileOutline>* self, u32 capacity);

struct UnkPolygonCollectionShape_ld62ec0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();  // updates the shape after a change
    u8 pad_0x8[0xa0];
    CRntVector<UnkTileOutline> outlines;  // 0xa8
    base::math::CVector2D min;            // 0xd0
    base::math::CVector2D max;            // 0xd8
    bool field_0xe0;                      // set while the outlines change
};
static_assert(__builtin_offsetof(UnkPolygonCollectionShape_ld62ec0, field_0xe0) == 0xe0, "layout");

// A point as the file stores it.
struct UnkPolygonVertex_ld62ec0 {
    f32 x, y;
    u32 field_0x8;
};

template <class T>
static inline void Read(CRntFile* file, T* value) {
    CRntFile_Read(file, value, sizeof(T), 1);
}

// Sets the outline count, building the new outlines and destroying the dropped ones.
static inline void Resize(CRntVector<UnkTileOutline>& v, u32 n) {
    if (v.count < n) {
        fn_710011f1ec(&v, n);
        for (u32 i = v.count; i < n; ++i)
            new (&v.data[i]) UnkTileOutline();
    } else if (v.count > n) {
        for (u32 i = n; i < v.count; ++i)
            v.data[i].~UnkTileOutline();
    } else {
        return;
    }
    v.count = n;
}

// The outline at `index`, growing the array to it when it may grow.
static inline UnkTileOutline& At(CRntVector<UnkTileOutline>& v, u32 index) {
    if (v.field_0x14 && v.count <= index) {
        u32 capacity = v.capacity;
        if (capacity <= index && v.step != 0)
            capacity = (index / v.step + 1) * v.step;
        fn_710011f1ec(&v, capacity);
        u32 n = index + 1;
        fn_710011f1ec(&v, n);
        for (u32 i = v.count; i < n; ++i)
            new (&v.data[i]) UnkTileOutline();
        v.count = n;
    }
    return v.data[index];
}

// Grows a point array's storage to at least `n`, copying the points over.
static inline void Reserve(CRntVector<UnkTilePoint>& v, u32 n) {
    if (v.capacity < n) {
        UnkTilePoint* p = static_cast<UnkTilePoint*>(v.allocator->vf_0x40((u64)n * sizeof(UnkTilePoint), 8, v.flags));
        if (v.data) {
            for (u32 i = 0; i < v.count; ++i)
                p[i] = v.data[i];
            v.allocator->vf_0x48(v.data);
        }
        v.data = p;
        v.capacity = n;
    }
}

void fn_7100d62ec0(UnkPolygonCollectionShape_ld62ec0* self, CRntFile* file) {
    self->outlines.Clear();
    self->field_0xe0 = true;
    u32 count = 0;
    Read(file, &count);
    Resize(self->outlines, count);
    self->field_0xe0 = true;

    for (u32 i = 0; i < count; ++i) {
        u32 point_count = 0;
        f32 z = 0.0f;
        Read(file, &point_count);
        Read(file, &z);

        UnkTileOutline& outline = At(self->outlines, i);
        outline.points.Clear();
        outline.field_0x28 = 0.0f;
        outline.field_0x2c = false;
        outline.field_0x2d = true;
        Reserve(outline.points, point_count);
        outline.field_0x28 = z;
        for (u32 j = 0; j < point_count; ++j) {
            base::math::CVector2D pos = {0.0f, 0.0f};
            u32 flags = 1;
            Read(file, &pos);
            Read(file, &flags);
            UnkPolygonVertex_ld62ec0 vertex = {pos.x, pos.y, flags};
            fn_7100118b94(&outline, reinterpret_cast<UnkVec3*>(&vertex));
        }

        bool closed = true;
        Read(file, &closed);
        fn_7100119f98(&outline, closed);
        base::math::CVector2D min = {0.0f, 0.0f};
        base::math::CVector2D max = {0.0f, 0.0f};
        Read(file, &min);
        Read(file, &max);
        outline.min = min;
        outline.max = max;
        outline.field_0x2d = false;
    }

    base::math::CVector2D min = {0.0f, 0.0f};
    base::math::CVector2D max = {0.0f, 0.0f};
    Read(file, &min);
    Read(file, &max);
    self->min = min;
    self->max = max;
    self->field_0xe0 = false;
    self->vf_0x80();
}
