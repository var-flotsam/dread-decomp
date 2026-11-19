// CBigFistAIComponent's unit (tools/tu_map.py unit 592): rebuilds the reach shape (a 2D polygon collection)
// from the component's path: a band 600 above the path's first and last segments widened by the fist's width,
// closed by two quarter-circle arcs (fn_71003e6530) of radius 540 down to each side's reach.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU108.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

using base::math::CVector2D;

struct UnkVec2;
struct UnkVec3;
struct UnkPointSet_b61;
void* fn_71003e6530(void* owner, const UnkVec2* origin, const UnkVec2* dir, void* sink, f32 radius, f32 limit);

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

struct UnkBigFistShape_l3e7d54 {
    u8 pad_0x0[0x98];
    bool field_0x98;  // set once the outline changed
};

struct UnkBigFistFist_l3e7d54 {
    u8 pad_0x0[0x528];
    f32 width;  // 0x528
};

struct UnkBigFistComponent_l3e7d54 {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkTilePoint* path;  // 0x68
    u32 path_count;      // 0x70
    u8 pad_0x74[0x20];
    bool path_closed;  // 0x94
    u8 pad_0x95[0x1333];
    f32 right_range;  // 0x13c8
    f32 left_range;   // 0x13cc
    u8 pad_0x13d0[0x135f0];
    UnkBigFistFist_l3e7d54* fist;  // 0x149c0

    f32 vf_0x3c8() { return reinterpret_cast<f32 (*)(UnkBigFistComponent_l3e7d54*)>(vtable[0x3c8 / 8])(this); }

    // An open path has one segment fewer than points.
    u32 SegmentCount() const { return path_count - (!path_closed && path_count != 0); }
    // Segment `i`: from point i to the next one, the last wrapping round to the first.
    UnkSegment2D Segment(u32 i) const {
        const UnkTilePoint* start = &path[i];
        const UnkTilePoint* end = i + 1 == path_count ? &path[0] : &path[i + 1];
        return UnkSegment2D(*reinterpret_cast<const CVector2D*>(start), *reinterpret_cast<const CVector2D*>(end));
    }
};
static_assert(__builtin_offsetof(UnkBigFistComponent_l3e7d54, right_range) == 0x13c8, "");
static_assert(__builtin_offsetof(UnkBigFistComponent_l3e7d54, fist) == 0x149c0, "");

static inline CVector2D Scale(const CVector2D& v, f32 s) {
    return {v.x * s, v.y * s};
}

static inline CVector2D Add(const CVector2D& a, const CVector2D& b) {
    return {a.x + b.x, a.y + b.y};
}

static inline CVector2D Neg(const CVector2D& v) {
    return {-v.x, -v.y};
}

static inline void AddPoint(UnkTileOutline* outline, UnkTilePoint* point) {
    fn_7100118b94(outline, reinterpret_cast<UnkVec3*>(point));
}

// Appends the arc's point `i` to `outline`.
static inline void AddArcPoint(UnkTileOutline* outline, UnkTileOutline* arc, UnkTilePoint* point, u32 i) {
    const UnkTilePoint* p =
        static_cast<const UnkTilePoint*>(fn_7100118950(reinterpret_cast<UnkPointSet_b61*>(arc), i));
    point->x = p->x;
    point->y = p->y;
    point->z = p->z;
    AddPoint(outline, point);
}

static inline void ResetArc(UnkTileOutline* arc) {
    arc->field_0x28 = 0.0f;
    arc->points.Clear();
    arc->field_0x2c = false;
    arc->field_0x2d = true;
}

bool fn_71003e7d54(UnkBigFistComponent_l3e7d54* self, UnkBigFistShape_l3e7d54* shape) {
    fn_7100d633ec(reinterpret_cast<UnkTileOutlines*>(shape));
    if (self->SegmentCount() == 0 || !self->fist)
        return false;

    f32 width = self->fist->width;
    UnkSegment2D first = self->Segment(0);
    UnkSegment2D last = self->Segment(self->SegmentCount() - 1);
    CVector2D left = Add(Neg(Scale({1.0f, 0.0f}, width)), first.start);
    CVector2D right = Add(Neg(Scale({-1.0f, 0.0f}, width)), last.end);

    CVector2D top_left = Add(left, {0.0f, 600.0f});
    CVector2D top_right = Add(right, {0.0f, 600.0f});

    UnkTileOutline outline;
    {
        UnkTilePoint corner = {top_left.x, top_left.y, 0.0f};
        AddPoint(&outline, &corner);
        corner = {top_right.x, top_right.y, 0.0f};
        AddPoint(&outline, &corner);
    }

    f32 left_base = self->vf_0x3c8();
    f32 right_base = self->vf_0x3c8();
    f32 left_reach = self->left_range < self->vf_0x3c8() ? 540.0f : left_base;
    f32 right_reach = self->right_range < self->vf_0x3c8() ? 540.0f : right_base;

    f32 right_limit = right_reach - width;
    f32 left_limit = left_reach - width;
    right_limit = right_reach - width;

    UnkTileOutline arc;
    UnkTilePoint point;
    point = {0.0f, 1.5707964f, 0.0f};
    fn_71003e6530(self, reinterpret_cast<const UnkVec2*>(&right), reinterpret_cast<const UnkVec2*>(&point), &arc,
                  540.0f, right_limit);
    for (s32 i = 0; i < (s32)arc.points.count; ++i)
        AddArcPoint(&outline, &arc, &point, i);

    ResetArc(&arc);
    point = {0.0f, -1.5707964f, 0.0f};
    fn_71003e6530(self, reinterpret_cast<const UnkVec2*>(&left), reinterpret_cast<const UnkVec2*>(&point), &arc,
                  540.0f, left_limit);
    for (s32 i = arc.points.count; i > 0;) {
        --i;
        AddArcPoint(&outline, &arc, &point, i);
    }

    fn_7100119f98(&outline, true);
    fn_7100d62d40(reinterpret_cast<UnkTileOutlines*>(shape), &outline);
    shape->field_0x98 = true;
    return true;
}
